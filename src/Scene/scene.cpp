#include <algorithm>
#include <random>

#include "scene.hpp"
#include "Object/Shape/shape.hpp"
#include "Object/Shape/sphere.hpp"
#include "Object/Shape/plane.hpp"
#include "Object/Light/light.hpp"
#include "Object/Camera/camera.hpp"
#include "Utils/utils.hpp"
#include "Utils/sampler.hpp"
#include "Image/image.hpp"
#include "Raytracer/ray.hpp"

Scene::Scene(Image* img) : _max_depth(0), _russian_roulette_coeff(1.0),
    _output_img(img) {}

Scene::Scene(unsigned depth, Image* img) : _max_depth(depth),
    _russian_roulette_coeff(1.0), _output_img(img) {}

Scene::~Scene() {

    for (Shape* s : _shapes)
        delete s;

    for (Light* l : _lights)
        delete l;

    for (Camera* c : _cameras)
        delete c;
}

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

std::string Scene::output_path() const
{
    return _output_img_path;
}

unsigned Scene::max_depth() const
{
    return _max_depth;
}

unsigned Scene::nb_samples() const
{
    return _nb_samples;
}

std::string Scene::mode() const
{
    return _mode;
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

std::string& Scene::output_path()
{
    return _output_img_path;
}

unsigned& Scene::max_depth()
{
    return _max_depth;
}

unsigned& Scene::nb_samples()
{
    return _nb_samples;
}

// Methods
void Scene::add(Object* o)
{
    Shape*  shape  { dynamic_cast<Shape*>(o)  };
    Light*  light  { dynamic_cast<Light*>(o)  };
    Camera* camera { dynamic_cast<Camera*>(o) };

    if (shape)
    {
        _shapes.push_back(shape);
        return;
    }

    if (light)
    {
        _lights.push_back(light);
        return;
    }

    if (camera)
    {
        _cameras.push_back(camera);
        return;
    }
}


//const Color Scene::compute_color(Ray &r)
//{
//    double ambient_light   { 0.1 };

//    Color obj_color        { r.intersection().material()->color()   };
//    Color ambient_color    { obj_color * ambient_light              };
//    Color diffuse_specular { compute_blinn_phong(r, obj_color)      };
//    Color reflect_color    { compute_refl_refractive(r)             };

//    return _russian_roulette_coeff *
//            (ambient_color + diffuse_specular + reflect_color).clamp();
//}

//const Color Scene::compute_refl_refractive(Ray &ray)
//{
//    Color refractive { Colors::BLACK };
//    Color reflective { Colors::BLACK };

//    Intersection i { ray.intersection() };

//    double R { 0 }, T { 0 };

//    // If the material is neither reflective nor refractive, we return black
//    if (i.material()->reflection() <= 0.0 && i.material()->refraction() <= 0.0)
//        return reflective + refractive;

//    // If there is no refraction, we just take the reflection ratio of the material into account
//    if (i.material()->refraction() <= 0.0)
//        R = i.material()->reflection();
//    else
//    {
//        /* For now we only consider reflection and refraction happening from air to
//         * a second medium. Hence n1 = 1 as an approximation. */
//        double n1       { 1.0                               };
//        double n2       { i.material()->refraction()        };
//        double cos_R    { -ray.direction().dot(i.normal())  };
//        double sin2_T   { (n1/n2)*(n1/n2)*(1 - cos_R*cos_R) };

//        R = schlick_approx(n1, n2, cos_R, sin2_T);
//        T = 1 - R;

//        if (T > 0.0)
//        {
//            double n { n1/n2 };
//            Vec3d refract_vect { n * ray.direction() +
//                        (n * cos_R - sqrt(1 - sin2_T))*i.normal() };
//            Ray refract_ray(i.position() + EPSILON*refract_vect, refract_vect);

//            refract_ray.bounces() = ray.bounces() + 1;

//            refractive = T * launch(refract_ray);
//        }
//    }

//    if (R > 0.0)
//    {
//        Vec3d reflect_vect;

//        std::random_device rnd_dv;
//        std::mt19937 gen(rnd_dv());
//        std::uniform_real_distribution<double> distrib(0.0, 1.0);

//        double u { distrib(gen) };

//        if (u <= R || _mode == "rt")  // Reflection is in the specular direction
//            reflect_vect = ray.direction().reflect(i.normal());
//        else // Reflection occurs in a random direction on the hemisphere (uniform sampling at the moment)
//        {
//            double v { distrib(gen) };

//            // Compute a random ray over the hemisphere at the intersection point
//            double r    { sqrt(1 - u*u) };
//            double phi  { 2 * PI * v    };

//            double x { cos(phi) * r };
//            double y { sin(phi) * r };

//            reflect_vect = Vec3d(x, y, u) - ray.intersection().position();

//            // The ray direction picked is in the wrong hemisphere
//            if (reflect_vect.dot(ray.intersection().normal()) < 0.0)
//                reflect_vect = reflect_vect.negative();
//        }

//        Ray reflect_ray(i.position() + EPSILON*reflect_vect, reflect_vect);

//        reflect_ray.bounces() = ray.bounces() + 1;

//        reflective = R * launch(reflect_ray);
//    }

//    return reflective + refractive;
//}
