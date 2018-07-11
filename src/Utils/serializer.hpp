#ifndef __SERIALIZER_HPP__
#define __SERIALIZER_HPP__

#include <string>

class Scene;
class QDomDocument;
class QDomElement;
class Shape;
class Sphere;
class Plane;
class Fluorescence;

class Serializer
{
public:
    Serializer();

    ~Serializer();

    void init(Scene *s);
    void write_to_XML(const std::string &output_path);
    Scene* read_from_XML(const std::string &input_path);
    Scene* scene() { return _scene; }

private:
    void serialize(QDomDocument &scene_doc, QDomElement &scene_elem, Shape* s);
    void serialize(QDomDocument &scene_doc, QDomElement &scene_elem, Sphere* s);
    void serialize(QDomDocument &scene_doc, QDomElement &scene_elem, Plane* p);

    void populate_scene_from_XML(const QDomElement &scene_element);

    const Fluorescence* read_fluorescence_component(QDomElement &fluo_elem);

    void add_camera(QDomElement &cam_elem);
    void add_light(QDomElement &light_elem);
    void add_shape(QDomElement &shape_elem, bool sphere);

    Scene *_scene;
};

#endif
