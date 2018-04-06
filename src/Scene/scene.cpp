#include <algorithm>

#include "scene.hpp"
#include "Object/Shape/shape.hpp"
#include "Object/Light/light.hpp"
#include "Object/Camera/camera.hpp"

#include "Raytracer/ray.hpp"

Scene::Scene() {}

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
    return *_shapes[i];
}

Light& Scene::light(unsigned i) const
{
    return *_lights[i];
}

Camera& Scene::camera(unsigned i) const
{
    return *_cameras[i];
}

// Methods
void Scene::add(Object* o)
{
    Shape*  shape   = dynamic_cast<Shape*>(o);
    Light*  light   = dynamic_cast<Light*>(o);
    Camera* camera  = dynamic_cast<Camera*>(o);

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

const Color Scene::compute_color(const Ray& r) const
{
    double ambiant_light = 0.2;

    Color obj_color          = r.intersection().material()->color();
    Color ambiant_color      = obj_color * ambiant_light;
    Color diffuse_specular   = compute_blinn_phong(r, obj_color);
    Color reflect_color      = compute_reflective(r);

    return (ambiant_color + diffuse_specular + reflect_color).clamp();
}

const Color Scene::compute_blinn_phong(const Ray &ray,
                                       const Color &obj_color) const
{
    Color diffuse   = Colors::BLACK;
    Color specular  = Colors::BLACK;

    const Intersection &i = ray.intersection();
    //const Material *mat = i.material();

    Vec3d towards_light =
            (_lights.at(0)->position() - i.position()).normalized();

    double source_distance = towards_light.magnitude();
    double cosine_n_l = i.normal().dot(towards_light);

    /* We need to compute the light contribution only if the normal points
     * towards the light */
    if (cosine_n_l > 0.0)
    {
        Ray shadow_ray(i.position() + EPSILON*towards_light, towards_light);
        bool in_shadow = false;
        double t;

        auto light_intersection =
                std::find_if(_shapes.begin(), _shapes.end(), [&] (Shape *shp) {
                    return shp->intersect(shadow_ray, t) && t < source_distance;
                });

        /* If the shadow ray intersects an object in the scene, we don't compute
         * the light contribution, because it's in the shadow */
        in_shadow = light_intersection != _shapes.end();

        if (!in_shadow)
        {
            diffuse  = (((obj_color * light(0).color()) * cosine_n_l)
                        + diffuse).clamp();
            specular = specular + compute_specular(ray, light(0));
        }
    }

    return diffuse + specular;
}

const Color Scene::compute_specular(const Ray &ray,
                             const Light &light) const
{
    Intersection i = ray.intersection();
    double shininess = i.material()->shininess();

    if (shininess <= 0.0)
        return Colors::BLACK;

    Vec3d view_vect  = (ray.origin() - i.position()).normalized();
    Vec3d light_vect = (light.position() - i.position()).normalized();
    Vec3d light_reflected = light_vect.reflect(i.normal());

    // Angle between the view vector and the reflected light
    double cosine_v_r = view_vect.dot(light_reflected);

    if (cosine_v_r <= 0.0)
        return Colors::BLACK;

    return pow(cosine_v_r, 5) * Colors::WHITE * light.color().a;
}

const Color Scene::compute_reflective(const Ray &ray) const
{
    return Colors::BLACK;
}
