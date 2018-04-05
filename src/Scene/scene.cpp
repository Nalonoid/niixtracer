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

void Scene::del(Object::OBJECT_TYPE obj_type, unsigned index)
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
    Intersection intersection = r.intersection();

    Vec3d towards_light =
            (_lights.at(0)->position() - intersection.position()).normalized();

    double source_distance = towards_light.magnitude();
    double cosine_n_l = intersection.normal().dot(towards_light);
    double ambiant_light = 0.2;

    Color obj_color = intersection.kd();
    Color light_contribution = obj_color * ambiant_light;

    /* We need to compute the light contribution only if the normal points
     * towards the light */
    if (cosine_n_l > 0.0)
    {
        Ray light_ray(intersection.position() + EPSILON*towards_light, towards_light);
        bool in_shadow = false;
        double t;

        auto light_intersection =
                std::find_if(_shapes.begin(), _shapes.end(), [&] (Shape *shp) {
                    return shp->intersect(light_ray, t) && t < source_distance;
                });

        /* If the shadow ray intersects an object in the scene, we don't compute
         * the light contribution, because it's in the shadow */
        in_shadow = light_intersection != _shapes.end();

        if (!in_shadow)
            light_contribution = (((obj_color * light(0).color()) * cosine_n_l)
                                 + light_contribution).clamp();
    }

    return light_contribution;
}
