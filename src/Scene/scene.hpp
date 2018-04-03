#ifndef __SCENE_HPP__
#define __SCENE_HPP__

#include <vector>
#include <memory>

#include "Math/math.hpp"

class Object;
class Shape;
class Light;
class Camera;

class Scene
{
public:
    Scene();

    // Getters
    const std::vector<Shape*>&  shapes() const;
    const std::vector<Light*>&  lights() const;
    const std::vector<Camera*>& cameras() const;

    const Shape&  shape(unsigned i) const;
    const Light&  light(unsigned i) const;
    const Camera& camera(unsigned i) const;

    // Methods
    template<typename... Args>
    void add(Object *o, Args... objs);

    void del(unsigned index);

private:
    void add(Object *o);

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
