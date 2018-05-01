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
    unsigned nb_samples() const;

    Shape&  shape(unsigned i) const;
    Light&  light(unsigned i) const;
    Camera& camera(unsigned i) const;

    std::string& output_path();
    unsigned& max_depth();
    unsigned& nb_samples();
    std::string mode() const;

    // Methods
    template<typename... Args>
    void add(Object *o, Args... objs);
    void add(Object *o);    // Terminate the recursion of add(Object *o, Args... objs)

    // Friends
    friend class Serializer;
    friend class Renderer;

    friend Scene* init_scene(unsigned index, Image *output_img, std::string mode,
                             unsigned samples_per_row_col, unsigned max_depth);

    // Members
private:
    std::vector<Shape*>  _shapes;
    std::vector<Light*>  _lights;
    std::vector<Camera*> _cameras;

    // Scene configuration infornation
    std::string _output_img_path;
    std::string _mode;
    unsigned    _max_depth;
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
