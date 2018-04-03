#include <algorithm>

#include "scene.hpp"
#include "Object/object.hpp"
#include "Object/Shape/shape.hpp"
#include "Object/Light/light.hpp"
#include "Object/Camera/camera.hpp"

Scene::Scene() {}

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


const Shape& Scene::shape(unsigned i) const
{
    return *_shapes[i];
}

const Light& Scene::light(unsigned i) const
{
    return *_lights[i];
}

const Camera& Scene::camera(unsigned i) const
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
        if (light)
        {
            _lights.push_back(light);
            if (camera)
                _cameras.push_back(camera);
        }
    }
}

void Scene::del(unsigned index)
{
//    auto it = std::find_if(_objects.begin(), _objects.end(),
//                            [&] (const Object *o) {
//                                return o->index() == index;
//                          });

//    if (it != _shapes.end())
//        _shapes.erase(it);
}
