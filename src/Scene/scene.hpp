#ifndef __SCENE_HPP__
#define __SCENE_HPP__

#include <vector>
#include <memory>

#include "Math/math.hpp"
#include "Object/object.hpp"

class Shape;
class Light;
class Camera;
class Color;
class Ray;
class Intersection;

class Scene
{
public:
    Scene();
    ~Scene();

    // Getters
    const std::vector<Shape*>&  shapes() const;
    const std::vector<Light*>&  lights() const;
    const std::vector<Camera*>& cameras() const;

    Shape&  shape(unsigned i) const;
    Light&  light(unsigned i) const;
    Camera& camera(unsigned i) const;

    // Methods
    template<typename... Args>
    void add(Object *o, Args... objs);

    void del(Object::OBJECT_TYPE obj_type, unsigned index);
    void add(Object *o);    // Terminate the recursion of add(Object *o, Args... objs)


    const Color compute_color(const Ray &r) const;
    const Color compute_blinn_phong(const Ray &ray, const Color &color) const;
    const Color compute_specular(const Ray &ray, const Light &light) const;
    const Color compute_reflective(const Ray &ray) const;

    // Members
private:
    std::vector<Shape*>     _shapes;
    std::vector<Light*>     _lights;
    std::vector<Camera*>    _cameras;
};

// Templated methods implementation must be in the .hpp file in order to compile
template<typename... Args>
void Scene::add(Object *o, Args... objs)
{
    add(o);
    add(objs...);
}

#endif
