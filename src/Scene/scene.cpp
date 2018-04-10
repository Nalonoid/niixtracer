#include <algorithm>

#include "scene.hpp"
#include "Object/Shape/shape.hpp"
#include "Object/Shape/sphere.hpp"
#include "Object/Shape/plane.hpp"
#include "Object/Light/light.hpp"
#include "Object/Camera/camera.hpp"
#include "Utils/utils.hpp"

#include "Raytracer/ray.hpp"

Scene::Scene() : _max_depth(2) {}

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
        std::cout << "LIGHT" << std::endl;

        _lights.push_back(light);
        return;
    }

    if (camera)
    {
        std::cout << "CAMERA" << std::endl;

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

const Color Scene::compute_color(Ray& r) const
{
    double ambiant_light   { 0.2 };

    Color obj_color        { r.intersection().material()->color() };
    Color ambiant_color    { obj_color * ambiant_light };
    Color diffuse_specular { compute_blinn_phong(r, obj_color) };
    Color reflect_color    { compute_reflective(r) };

    return (ambiant_color + diffuse_specular + reflect_color).clamp();
;
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

    Vec3d view_vect  { (ray.origin() - i.position()).normalized()       };
    Vec3d light_vect { (i.position() - light.position()).normalized()   };

    /* No need to normalize here because light_vect is normalized already and
     * |r|^2 == |l|^2 == 1 */
    Vec3d refl_vect  { light_vect.reflect(i.normal()) };

    // Angle between the view vector and the reflected light
    double cosine_v_r { view_vect.dot(refl_vect) };

    if (cosine_v_r > 0.0)
        specular = pow(cosine_v_r, 0.25*shininess) * light.color();

    return specular;
}

const Color Scene::compute_reflective(Ray &ray) const
{
    Color refractive { Colors::BLACK };
    Color reflective { Colors::BLACK };

    Intersection i { ray.intersection() };

    /* For now we only consider reflection and refraction happening from air to
     * a second medium. Hence n1 = 1 as an approximation and n1 always <= n2. */
    double n1 { 1.0                        };
    double n2 { i.material()->refraction() };
    double R0 { (n1 - n2)/(n1 + n2)        };

    R0 *= R0;

    Vec3d view_vect     { ray.direction().negative() };
    Vec3d reflect_vect  { ray.direction().reflect(i.normal()) };

    double R = R0 + (1 - R0) * pow(1 - view_vect.dot(i.normal()), 5);
    double T = 1 - R;

    if (R > 0.0)
    {
        Ray reflect_ray(i.position(), reflect_vect);

        if ((reflect_ray.bounces() = ray.bounces() + 1) > _max_depth)
            return reflective;

        reflective = this->launch(reflect_ray) * R;
    }

    if (T > 0.0)
        refractive = Colors::BLACK * T;

    return reflective + refractive;
}
