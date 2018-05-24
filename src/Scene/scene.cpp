#include <algorithm>
#include <random>

#include "scene.hpp"
#include "Object/Shape/shape.hpp"
#include "Object/Shape/sphere.hpp"
#include "Object/Shape/plane.hpp"
#include "Object/Light/light.hpp"
#include "Object/Camera/camera.hpp"
#include "Utils/sampler.hpp"
#include "Image/image.hpp"
#include "Raytracer/ray.hpp"

Scene::Scene(Image* img) : _max_depth(0), _russian_roulette_coeff(1.0),
    _output_img(img) {}

Scene::Scene(unsigned depth, Image* img) : _max_depth(depth),
    _russian_roulette_coeff(1.0), _output_img(img) {}

Scene::~Scene()
{
    delete _output_img;

    for (Shape* s : _shapes)
        delete s;

    for (Light* l : _lights)
        delete l;

    for (Camera* c : _cameras)
        delete c;
}

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

std::string Scene::output_path() const
{
    return _output_img_path;
}

unsigned Scene::max_depth() const
{
    return _max_depth;
}

unsigned Scene::nb_samples() const
{
    return _nb_samples;
}

std::string Scene::mode() const
{
    return _mode;
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

std::string& Scene::output_path()
{
    return _output_img_path;
}

unsigned& Scene::max_depth()
{
    return _max_depth;
}

unsigned& Scene::nb_samples()
{
    return _nb_samples;
}

// Methods
void Scene::add(Object* o)
{
    Shape   *shape  { dynamic_cast<Shape*>(o)  };
    Light   *light  { dynamic_cast<Light*>(o)  };
    Camera  *camera { dynamic_cast<Camera*>(o) };

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
