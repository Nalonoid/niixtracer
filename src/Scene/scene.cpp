#include <algorithm>

#include "scene.hpp"
#include "Object/Shape/shape.hpp"
#include "Object/Shape/sphere.hpp"
#include "Object/Shape/plane.hpp"
#include "Object/Light/light.hpp"
#include "Object/Camera/camera.hpp"
#include "Utils/utils.hpp"

#include "Raytracer/ray.hpp"

Scene::Scene() : _max_depth(1) {}

Scene::~Scene() {}

// Getters
const std::vector<Shape*>& Scene::shapes() const
{
    return _shapes;
}

const std::vector<Light*>& Scene::lights() const
{
    return _lights;
}

const std::vector<Camera*>& Scene::cameras() const
{
    return _cameras;
}


Shape& Scene::shape(unsigned i) const
{
    return *(_shapes[i]);
}

Light& Scene::light(unsigned i) const
{
    return *(_lights[i]);
}

Camera& Scene::camera(unsigned i) const
{
    return *(_cameras[i]);
}

// Methods
void Scene::add(Object* o)
{
    Shape*  shape  { dynamic_cast<Shape*>(o)  };
    Light*  light  { dynamic_cast<Light*>(o)  };
    Camera* camera { dynamic_cast<Camera*>(o) };

    if (shape)
    {
        _shapes.push_back(shape);
        return;
    }

    if (light)
    {
        _lights.push_back(light);
        return;
    }

    if (camera)
    {
        _cameras.push_back(camera);
        return;
    }
}

void Scene::del(Object::OBJECT_TYPE, unsigned)
{
//    auto it = std::find_if(_objects.begin(), _objects.end(),
//                            [&] (const Object *o) {
//                                return o->index() == index;
//                          });

//    if (it != _shapes.end())
//        _shapes.erase(it);
}

Color Scene::launch(Ray &ray) const
{
    Color contrib { Colors::BLACK };
    bool collides { false };

    for (auto itr = _shapes.begin(); itr < _shapes.end(); itr++)
        collides = (*itr)->intersect(ray) || collides;

    if (collides)
        return compute_color(ray);

    return contrib;
}

const Color Scene::compute_color(Ray &r) const
{
    double ambiant_light   { 0.1 };

    Color obj_color        { r.intersection().material()->color() };
    Color ambiant_color    { obj_color * ambiant_light };
    Color diffuse_specular { compute_blinn_phong(r, obj_color) };
    Color reflect_color    { compute_refl_refractive(r) };

    return (ambiant_color + diffuse_specular + reflect_color).clamp();
}

const Color Scene::compute_blinn_phong(Ray &ray, const Color &obj_color) const
{
    Color diffuse  { Colors::BLACK };
    Color specular { Colors::BLACK };

    const Intersection &i { ray.intersection() };

    for (unsigned light_id {0}; light_id < _lights.size(); ++light_id)
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
                    std::find_if(_shapes.begin(), _shapes.end(), [&] (Shape *shp) {
                        return shp->intersect(shadow_ray)
                               && shadow_ray.dist_max() < source_distance;
                    });

            in_shadow = light_intersection != _shapes.end();

            if (!in_shadow)
            {
                diffuse = ((obj_color * cosine_n_l) + diffuse)
                        * light(light_id).brightness();

                /* In case of a glossy material we need to compute the specular
                 * color. We add this contribution to the diffuse color. */
                if (i.material()->shininess() > 0)
                    specular = specular + compute_specular(ray, light(light_id));
            }
        }
    }

    return diffuse + specular;
}

const Color Scene::compute_specular(Ray &ray, const Light &light) const
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

    // Cosine of the angle between the view vector and the reflected light
    double cosine_v_r { view_vect.dot(refl_vect) };

    if (cosine_v_r > 0.0)
        specular = pow(cosine_v_r, shininess) * light.color();

    return specular;
}

const Color Scene::compute_refl_refractive(Ray &ray) const
{
    Color refractive { Colors::BLACK };
    Color reflective { Colors::BLACK };

    Intersection i { ray.intersection() };

    double R { 0 }, T { 0 };

    // If the material is neither reflective nor refractive, we return black
    if (i.material()->reflection() <= 0.0 && i.material()->refraction() <= 0.0)
        return reflective + refractive;

    // If we reached the maximum number of recursive reflections/refractions
    if (ray.bounces() == _max_depth)
        return reflective + refractive;

    // If there is no refraction, we just take the reflection ratio of the material into account
    if (i.material()->refraction() <= 0.0)
        R = i.material()->reflection();
    else
    {
        /* For now we only consider reflection and refraction happening from air to
         * a second medium. Hence n1 = 1 as an approximation. */
        double n1       { 1.0                               };
        double n2       { i.material()->refraction()        };
        double cos_R    { -ray.direction().dot(i.normal())  };
        double sin2_T   { (n1/n2)*(n1/n2)*(1 - cos_R*cos_R) };

        R = schlick_approx(n1, n2, cos_R, sin2_T);
        T = 1 - R;

        if (T > 0.0)
        {
            double n = n1/n2;
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

double Scene::schlick_approx(double n1, double n2, double cos_R, double sin2_T) const
{
    double R { 0 };

    // If sin2(thetaT) > 1 we're in the case of Total Internal Reflection
    if (n1 > n2 && sin2_T > 1.0)
        R = 1.0;
    else
    {
        // Cosine of : reflected vector with n or transmitted vector with n
        double used_cos { n1 > n2 ? sqrt(1 - sin2_T) : cos_R };
        double R0       { (n1 - n2)/(n1 + n2)                };
        R0 *= R0;

        R = R0 + (1 - R0) * pow(1 - used_cos, 5);
    }

    return R;
}
