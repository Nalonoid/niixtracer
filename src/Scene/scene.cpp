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
    float ambiant_light = 0.2;
    Intersection i = r.intersection();

    Color light_contribution = i.kd();
    light_contribution.scale(ambiant_light);

    Vec3d towards_light = _lights.at(0)->position() - i.position();
    double source_distance = towards_light.magnitude();
    towards_light = towards_light.normalized();

    double cosine_n_l = i.normal().dot(towards_light);

    /* We need to compute the light contribution only if the normal points
     * towards the light */
    if (cosine_n_l > 0.0)
    {
        Ray light_ray(i.position() + EPSILON*towards_light, towards_light);
        double t;

        auto light_intersection =
                std::find_if(_shapes.begin(), _shapes.end(), [&] (Shape *shp) {
                    return shp->intersect(light_ray, t) && t < source_distance;
                });

        /* If the shadow ray does not intersect an object of the scene, we
         * compute the light contribution */
        if (light_intersection == _shapes.end())
            return i.kd();
    }

    return light_contribution;
}
