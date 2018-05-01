#include "raytracer.hpp"
#include "ray.hpp"
#include "Image/color.hpp"
#include "Scene/scene.hpp"
#include "Object/Light/light.hpp"
#include "Object/Shape/shape.hpp"

#include <algorithm>

Raytracer::Raytracer(Scene *scene) : Renderer(scene) {}

bool Raytracer::depth_recursion_over(Ray &ray)
{
    return ray.bounces() > _scene->max_depth();
}

Color Raytracer::compute_color(Ray &ray)
{
    double ambient_light   { 0.1 };

    Color obj_color        { ray.intersection().material()->color() };
    Color ambient_color    { obj_color * ambient_light              };
    Color diffuse_specular { compute_blinn_phong(ray, obj_color)    };
    Color reflect_color    { compute_refl_refractive(ray)           };

    return (ambient_color + diffuse_specular + reflect_color).clamp();
}

const Color Raytracer::compute_blinn_phong(Ray &ray, const Color &obj_color) const
{
    Color diffuse  { Colors::BLACK };
    Color specular { Colors::BLACK };

    const Intersection &i { ray.intersection() };

    for (unsigned light_id {0}; light_id < lights().size(); ++light_id)
    {
        Vec3d towards_light     { light(light_id).position() - i.position() };
        double source_distance  { towards_light.magnitude() };

        towards_light = towards_light.normalized();

        double cosine_n_l { i.normal().dot(towards_light) };

        /* We need to compute the light contribution only if the normal points
         * towards the light */
        if (cosine_n_l > 0.0)
        {
            Ray shadow_ray(i.position() + EPSILON*towards_light, towards_light);
            bool in_shadow { false };

            /* Intersected objects must be between the light source and the
             * first intersection in order to cast a shadow. */
            auto light_intersection =
                    std::find_if(shapes().begin(), shapes().end(), [&](Shape *shp)
                    {
                        return shp->intersect(shadow_ray)
                               && shadow_ray.dist_max() < source_distance;
                    });

            in_shadow = light_intersection != shapes().end();

            if (!in_shadow)
            {
                diffuse = ((obj_color * cosine_n_l) + diffuse)
                        * light(light_id).brightness();

                /* In case of a glossy material we need to compute the specular
                 * color. We add this contribution to the diffuse color. */
                if (i.material()->shininess() > 0)
                    specular += compute_specular(ray, light(light_id));
            }
        }
    }

    return diffuse + specular;
}

const Color Raytracer::compute_specular(Ray &ray, const Light &light) const
{
    Color specular { Colors::BLACK };

    const Intersection &i { ray.intersection() };
    double shininess { i.material()->shininess() };

    // The surface is not shiny, so we do not compute specular color
    if (shininess == 0.0)
        return specular;

    Vec3d view_vect  { (ray.origin() - i.position()).normalized()       };
    Vec3d light_vect { (i.position() - light.position()).normalized()   };

    /* No need to normalize here because light_vect is normalized already and
     * |r|^2 == |l|^2 == 1 */
    Vec3d refl_vect  { light_vect.reflect(i.normal()) };

    double cosine_v_r { view_vect.dot(refl_vect) };

    if (cosine_v_r > 0.0)
        specular = pow(cosine_v_r, shininess*0.25) * light.color();

    return specular;
}

const Color Raytracer::compute_refl_refractive(Ray &ray)
{
    Color refractive { Colors::BLACK };
    Color reflective { Colors::BLACK };

    Intersection i { ray.intersection() };

    double R { 0 }, T { 0 };

    // If the material is neither reflective nor refractive, we return black
    if (i.material()->reflection() <= 0.0 && i.material()->refraction() <= 0.0)
        return reflective + refractive;

    // If there is no refraction, we simply take the reflection ratio of the material into account
    if (i.material()->refraction() <= 0.0)
        R = i.material()->reflection();
    else
    {
        /* For now we only consider reflection and refraction happening from air to
         * a second medium. Hence n1 = 1 as an approximation.
         * TO-DO: modify Intersection to handle rays passing from one media to another
         * e.g. Intersection::through_material and Intersection::end_material */

        double n1       { 1.0                               };
        double n2       { i.material()->refraction()        };
        double cos_R    { -ray.direction().dot(i.normal())  };
        double sin2_T   { (n1/n2)*(n1/n2)*(1 - cos_R*cos_R) };

        R = schlick_approx(n1, n2, cos_R, sin2_T);
        T = 1 - R;

        if (T > 0.0)
        {
            double n { n1/n2 };
            Vec3d refract_vect { n * ray.direction() +
                        (n * cos_R - sqrt(1 - sin2_T))*i.normal() };
            Ray refract_ray(i.position() + EPSILON*refract_vect, refract_vect);

            refract_ray.bounces() = ray.bounces() + 1;

            refractive = T * launch(refract_ray);
        }
    }

    if (R > 0.0)
    {
        Vec3d reflect_vect { ray.direction().reflect(i.normal()) };
        Ray reflect_ray(i.position() + EPSILON*reflect_vect, reflect_vect);

        reflect_ray.bounces() = ray.bounces() + 1;

        reflective = R * launch(reflect_ray);
    }

    return reflective + refractive;
}
