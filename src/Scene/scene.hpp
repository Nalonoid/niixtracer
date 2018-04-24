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
    Scene(Image *img);
    Scene(unsigned depth, Image *img);

    ~Scene();

    // Getters
    const std::vector<Shape*>&  shapes() const;
    const std::vector<Light*>&  lights() const;
    const std::vector<Camera*>& cameras() const;

    std::string output_path() const;
    unsigned max_depth() const;
    const std::string& mode() const;
    unsigned nb_samples() const;

    Shape&  shape(unsigned i) const;
    Light&  light(unsigned i) const;
    Camera& camera(unsigned i) const;

    unsigned& max_depth();
    std::string& mode();
    unsigned& nb_samples();

    // Methods
    template<typename... Args>
    void add(Object *o, Args... objs);
    void add(Object *o);    // Terminate the recursion of add(Object *o, Args... objs)

    Color launch(Ray &ray);
    void render(unsigned i, unsigned j);
    bool depth_recursion_over(Ray &ray);

    const Color compute_color(Ray &r);
    const Color compute_blinn_phong(Ray &ray, const Color &color) const;
    const Color compute_specular(Ray &ray, const Light &light) const;
    const Color compute_refl_refractive(Ray &ray);

    double schlick_approx(double n1, double n2, double cos_R, double sin2_T) const;

    // Friends
    friend class Serializer;

    friend Scene* init_scene(unsigned index, Image *output_img, std::string& mode,
                             unsigned samples_per_row_col, unsigned max_depth);

    // Members
private:
    std::vector<Shape*>  _shapes;
    std::vector<Light*>  _lights;
    std::vector<Camera*> _cameras;

    // Scene configuration infornation
    std::string _output_img_path;
    unsigned    _max_depth;
    std::string _mode;
    unsigned    _nb_samples;
    double      _russian_roulette_coeff;
    Image*      _output_img;
};

// Templated methods implementation must be in the .hpp file in order to compile
template<typename... Args>
void Scene::add(Object *o, Args... objs)
{
    add(o);
    add(objs...);
}

#endif
