#include <algorithm>

#include "scene.hpp"
#include "Object/Shape/shape.hpp"
#include "Object/Shape/sphere.hpp"
#include "Object/Shape/plane.hpp"
#include "Object/Light/light.hpp"
#include "Object/Camera/camera.hpp"

#include "Raytracer/ray.hpp"

Scene::Scene() {}

Scene::~Scene() {}

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

// Methods
void Scene::add(Object* o)
{
    Shape*  shape   = dynamic_cast<Shape*>(o);
    Light*  light   = dynamic_cast<Light*>(o);
    Camera* camera  = dynamic_cast<Camera*>(o);

    if (shape)
    {
        std::cout << "SHAPE" << std::endl;
        Sphere *s = dynamic_cast<Sphere*>(shape);
        if (s)
            std::cout << "SPHERE!" << std::endl;

        Plane *p = dynamic_cast<Plane*>(shape);
        if (p)
            std::cout << "PLANE!" << std::endl;

        _shapes.push_back(shape);
        return;
    }

    if (light)
    {
        std::cout << "LIGHT" << std::endl;

        _lights.push_back(light);
        return;
    }

    if (camera)
    {
        std::cout << "CAMERA" << std::endl;

        _cameras.push_back(camera);
        return;
    }
}

void Scene::del(Object::OBJECT_TYPE, unsigned)
{
//    auto it = std::find_if(_objects.begin(), _objects.end(),
//                            [&] (const Object *o) {
//                                return o->index() == index;
//                          });

//    if (it != _shapes.end())
//        _shapes.erase(it);
}

const Color Scene::compute_color(const Ray& r) const
{
    double ambiant_light = 0.2;

    Color obj_color          = r.intersection().material()->color();
    Color ambiant_color      = obj_color * ambiant_light;
    Color diffuse_specular   = compute_blinn_phong(r, obj_color);
    Color reflect_color      = compute_reflective(r);

    return (ambiant_color + diffuse_specular + reflect_color).clamp();
}

const Color Scene::compute_blinn_phong(const Ray &ray,
                                       const Color &obj_color) const
{
    Color diffuse  = Colors::BLACK;
    Color specular = Colors::BLACK;

    const Intersection &i = ray.intersection();

    Vec3d towards_light = light(0).position() - i.position();
    double source_distance = towards_light.magnitude();
    towards_light = towards_light.normalized();

    double cosine_n_l = i.normal().dot(towards_light);

    /* We need to compute the light contribution only if the normal points
     * towards the light */
    if (cosine_n_l > 0.0)
    {
        Ray shadow_ray(i.position() + EPSILON*towards_light, towards_light);
        bool in_shadow = false;

        auto light_intersection =
                std::find_if(_shapes.begin(), _shapes.end(), [&] (Shape *shp) {
                    return shp->intersect(shadow_ray)
                           && shadow_ray.dist_max() < source_distance;
                });

        in_shadow = light_intersection != _shapes.end();

        if (!in_shadow)
        {
            diffuse  = ((obj_color * cosine_n_l) + diffuse) * light(0).color().brightness();

            if (i.material()->shininess() > 0)
                specular = specular + compute_specular(ray, light(0));
        }
    }

    return diffuse + specular;
}

const Color Scene::compute_specular(const Ray &ray,
                             const Light &light) const
{
    Color specular = Colors::BLACK;
    Intersection i = ray.intersection();
    double shininess = i.material()->shininess();

    Vec3d view_vect  = (ray.origin() - i.position()).normalized();
    Vec3d light_vect = (i.position() - light.position()).normalized();
    Vec3d light_reflected = light_vect.reflect(i.normal().normalized());

    // Angle between the view vector and the reflected light
    double cosine_v_r = view_vect.dot(light_reflected);

    if (cosine_v_r > 0.0)
        specular = pow(cosine_v_r, 10) * light.color() * shininess;

    return specular;
}

const Color Scene::compute_reflective(const Ray &ray) const
{
    return Colors::BLACK;
}
