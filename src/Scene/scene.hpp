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
class Image;

class Scene
{
public:
    Scene();
    Scene(unsigned depth);

    ~Scene();

    // Getters
    const std::vector<Shape*>&  shapes() const;
    const std::vector<Light*>&  lights() const;
    const std::vector<Camera*>& cameras() const;

    unsigned max_depth() const;
    const std::string& mode() const;
    unsigned nb_samples() const;
    const Image* output_image_p() const;

    Shape&  shape(unsigned i) const;
    Light&  light(unsigned i) const;
    Camera& camera(unsigned i) const;

    Image** output_image_p();
    unsigned& max_depth();
    std::string& mode();
    unsigned& nb_samples();

    // Methods
    template<typename... Args>
    void add(Object *o, Args... objs);
    void add(Object *o);    // Terminate the recursion of add(Object *o, Args... objs)

    Color launch(Ray &ray) const;
    void render(unsigned i, unsigned j) const;

    const Color compute_color(Ray &r) const;
    const Color compute_blinn_phong(Ray &ray, const Color &color) const;
    const Color compute_specular(Ray &ray, const Light &light) const;
    const Color compute_refl_refractive(Ray &ray) const;

    double schlick_approx(double n1, double n2, double cos_R, double sin2_T) const;

    // Members
private:
    std::vector<Shape*>  _shapes;
    std::vector<Light*>  _lights;
    std::vector<Camera*> _cameras;

    Image*      _output_img;
    unsigned    _max_depth;
    std::string _mode;
    unsigned    _nb_samples;
};

// Templated methods implementation must be in the .hpp file in order to compile
template<typename... Args>
void Scene::add(Object *o, Args... objs)
{
    add(o);
    add(objs...);
}

#endif
