#ifndef __RENDERER_HPP__
#define __RENDERER_HPP__

class Ray;
class Color;
class Scene;
class Shape;
class Light;
class Camera;

#include <vector>
#include <chrono>

class Renderer
{
public:
    Renderer(Scene *scene);
    virtual ~Renderer();

    void  render_scene();

    const std::vector<Shape*>&  shapes()  const;
    const std::vector<Light*>&  lights()  const;
    const std::vector<Camera*>& cameras() const;

    Shape&  shape(unsigned i) const;
    Light&  light(unsigned i) const;
    Camera& camera(unsigned i) const;

protected:
    Color           launch(Ray &ray);
    virtual bool    depth_recursion_over(Ray &ray) = 0;
    virtual Color   compute_color(Ray &ray) = 0;

    Scene* _scene;

};

#endif
