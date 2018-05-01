#include <algorithm>

#include "pathtracer.hpp"
#include "ray.hpp"

#include "Image/color.hpp"
#include "Scene/scene.hpp"
#include "Object/Light/light.hpp"
#include "Object/Shape/shape.hpp"
#include "Utils/sampler.hpp"

Pathtracer::Pathtracer(Scene *scene) : Renderer(scene),
    _russian_roulette_coeff(1.0f) {}

/* https://computergraphics.stackexchange.com/questions/2316/is-russian-roulette-really-the-answer
 * see the above link for a nice explanation on how russian roulettes works and
 * why it is mathematically unbiased */
bool Pathtracer::depth_recursion_over(Ray &ray)
{
    unsigned curr_depth { ray.bounces() };

    _russian_roulette_coeff = 1.0;

    if (curr_depth > _scene->max_depth())
    {
        Uniform sampler;

        double u                { sampler.sample()  };
        double rr_stop_proba    { 0.1               };

        if (u > rr_stop_proba)
            _russian_roulette_coeff /= (1.0 - rr_stop_proba);
    }

    return curr_depth > _scene->max_depth() && _russian_roulette_coeff == 1.0;
}

// given v1, set v2 and v3 so they form an orthonormal system
// (we assume v1 is already normalized)
void ons(const Vec3d& v1, Vec3d& v2, Vec3d& v3) {
    if (std::abs(v1.x) > std::abs(v1.y)) {
        // project to the y = 0 plane and construct a normalized orthogonal vector in this plane
        float invLen = 1.f / sqrtf(v1.x * v1.x + v1.z * v1.z);
        v2 = Vec3d(-v1.z * invLen, 0.0f, v1.x * invLen);
    } else {
        // project to the x = 0 plane and construct a normalized orthogonal vector in this plane
        float invLen = 1.0f / sqrtf(v1.y * v1.y + v1.z * v1.z);
        v2 = Vec3d(0.0f, v1.z * invLen, -v1.y * invLen);
    }
    v3 = v1.cross(v2);
}

Color Pathtracer::compute_color(Ray &ray)
{
    const Intersection &i   { ray.intersection()    };
    const Material *m       { i.material()          };
    const Shape *s          { i.shape()             };
    const Color &obj_col    { m->color()            };

    Color ret_color;

    switch (m->type())
    {
    case MATERIAL_TYPE::DIFFUSE:
    {
        Vec3d rotX, rotY;
        ons(i.normal(), rotX, rotY);

        Vec3d hsphere_smpl  { hemisphere_sample()           };
        Vec3d recursive_dir {
            Vec3d(rotX.x, rotY.x, i.normal().x).dot(hsphere_smpl),
            Vec3d(rotX.y, rotY.y, i.normal().y).dot(hsphere_smpl),
            Vec3d(rotX.z, rotY.z, i.normal().z).dot(hsphere_smpl) };

        Ray recursive_ray(i.position() + EPSILON*recursive_dir, recursive_dir);
        recursive_ray.bounces() = ray.bounces() + 1;

        double cos_att { recursive_dir.dot(i.normal()) };

        ret_color = Color(s->emission()) * _russian_roulette_coeff +
                obj_col * launch(recursive_ray) * cos_att *
                _russian_roulette_coeff;
        break;
    }
    case MATERIAL_TYPE::SPECULAR:
    {
        Vec3d reflect_vect;
        double R { i.material()->reflection() };

        Uniform sampler;
        double u { sampler.sample() };

        if (u <= R) // The reflection is in the perfect specular direction
            reflect_vect = ray.direction().reflect(i.normal());
        else        // Here we take a random direction on the hemisphere
        {
            Vec3d rotX, rotY;
            ons(i.normal(), rotX, rotY);

            Vec3d hsphere_smpl  { hemisphere_sample()           };
            reflect_vect = Vec3d(
                Vec3d(rotX.x, rotY.x, i.normal().x).dot(hsphere_smpl),
                Vec3d(rotX.y, rotY.y, i.normal().y).dot(hsphere_smpl),
                Vec3d(rotX.z, rotY.z, i.normal().z).dot(hsphere_smpl));
        }

        Ray reflection_ray(i.position() + EPSILON * reflect_vect, reflect_vect);
        reflection_ray.bounces() = ray.bounces() + 1;

        ret_color = Color(s->emission()) * _russian_roulette_coeff +
                launch(reflection_ray) * _russian_roulette_coeff;
        break;
    }
    case MATERIAL_TYPE::REFRACTIVE:
    {
        break;
    }
    default:
        break;
    }

    return ret_color;
}

const Color Pathtracer::compute_refl_refractive(Ray &ray)
{
    Color refractive { Colors::BLACK };
    Color reflective { Colors::BLACK };

    Intersection i { ray.intersection() };

    double R { 0 }, T { 0 };

    // If the material is neither reflective nor refractive, we return the obj color
    if (i.material()->reflection() <= 0.0 && i.material()->refraction() <= 0.0)
        return reflective + refractive;

    Vec3d   recursive_dir;
    Ray     recursive_ray;

    /* If there is no refraction, we just take the reflection ratio of the
     * material into account */
    if (i.material()->refraction() <= 0.0)
        R = i.material()->reflection();
    else
    {
        /* For now we only consider reflection and refraction happening from air
         * to a second medium. Hence n1 = 1 as an approximation. */
        double n1       { 1.0                               };
        double n2       { i.material()->refraction()        };
        double cos_R    { -ray.direction().dot(i.normal())  };
        double sin2_T   { (n1/n2)*(n1/n2)*(1 - cos_R*cos_R) };

        R = schlick_approx(n1, n2, cos_R, sin2_T);
        T = 1 - R;

        if (T > 0.0)
        {
            double n { n1/n2 };
            recursive_dir = n * ray.direction() +
                    (n * cos_R - sqrt(1 - sin2_T))*i.normal();
            recursive_ray =
                    Ray(i.position() + EPSILON*recursive_dir, recursive_dir);

            recursive_ray.bounces() = ray.bounces() + 1;

            refractive = T * launch(recursive_ray);
        }
    }

    double refract_attenuation { recursive_ray.direction().dot(i.normal()) };

    if (R > 0.0)
    {
        Uniform sampler;

        double u { sampler.sample() };

        if (u <= R)  // Reflection is in the specular direction
            recursive_dir = ray.direction().reflect(i.normal());
        else // Reflection occurs in a random direction on the hemisphere (uniform sampling at the moment)
        {
            double v { sampler.sample() };

            // Compute a random ray over the hemisphere at the intersection point
            double r    { sqrt(1 - u*u) };
            double phi  { 2 * PI * v    };

            double x { cos(phi) * r };
            double y { sin(phi) * r };

            recursive_dir = Vec3d(x, y, u) - ray.intersection().position();

            // The ray direction picked is in the wrong hemisphere
            if (recursive_dir.dot(ray.intersection().normal()) < 0.0)
                recursive_dir = recursive_dir.negative();
        }

        recursive_ray = Ray(i.position() + EPSILON*recursive_dir, recursive_dir);

        recursive_ray.bounces() = ray.bounces() + 1;

        reflective = R * launch(recursive_ray);
    }

    double reflect_attenuation { recursive_ray.direction().dot(i.normal()) };

    return reflect_attenuation * reflective + refract_attenuation * refractive;
}
