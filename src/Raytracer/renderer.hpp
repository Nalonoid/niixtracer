#ifndef __RENDERER_HPP__
#define __RENDERER_HPP__

class Ray;
class Color;
class Scene;
class Shape;
class Light;
class Camera;

#include <vector>

class Renderer
{
public:
    Renderer(Scene *scene);

    void  render();
    Color launch(Ray &ray);

    virtual bool depth_recursion_over(Ray &ray) = 0;
    virtual Color compute_color(Ray &ray) = 0;

    const std::vector<Shape*>&  shapes()  const;
    const std::vector<Light*>&  lights()  const;
    const std::vector<Camera*>& cameras() const;

    Shape&  shape(unsigned i) const;
    Light&  light(unsigned i) const;
    Camera& camera(unsigned i) const;

protected:
    const Scene* _scene;

};

#endif
