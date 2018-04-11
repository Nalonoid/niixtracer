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
    void add(Object *o);    // Terminate the recursion of add(Object *o, Args... objs)
    void del(Object::OBJECT_TYPE obj_type, unsigned index);

    Color launch(Ray &ray) const;

    const Color compute_color(Ray &r) const;
    const Color compute_blinn_phong(Ray &ray, const Color &color) const;
    const Color compute_specular(Ray &ray, const Light &light) const;
    const Color compute_reflective(Ray &ray) const;
    const Color compute_refractive(Ray &ray) const;

    double schlick_approx(double n1, double n2,
                          Vec3d incident, Vec3d normal) const;

    // Members
private:
    std::vector<Shape*>  _shapes;
    std::vector<Light*>  _lights;
    std::vector<Camera*> _cameras;

    unsigned _max_depth;
};

// Templated methods implementation must be in the .hpp file in order to compile
template<typename... Args>
void Scene::add(Object *o, Args... objs)
{
    add(o);
    add(objs...);
}

#endif
