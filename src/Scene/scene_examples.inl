#include "Scene/scene.hpp"
#include "Object/Camera/camera.hpp"
#include "Object/Light/light.hpp"
#include "Object/Shape/sphere.hpp"
#include "Object/Shape/plane.hpp"

namespace Scenes
{

Scene SCENE_0(2, new Image(1600, 900));
Scene SCENE_1(2, new Image(1600, 900));

}

Scene* init_scene(unsigned index, Image *output_img, std::string mode,
                  unsigned samples_per_row_col, unsigned max_depth)
{
    Scene *s;

    switch (index) {
    case 0:
        Scenes::SCENE_0.add(
                dynamic_cast<Object*>(new Camera(Vec3d(0, 3, -10), Space::ORIGIN))

                ,dynamic_cast<Object*>(new Light(Vec3d(6, 5, -6), Colors::WHITE))

                ,dynamic_cast<Object*>(new Plane(Vec3d(0, 0, -1), -10.0, Colors::BLUE, Materials::DIFFUSE))
                ,dynamic_cast<Object*>(new Plane(Vec3d(0, 1,  0),   0.0, Colors::MAROON, Materials::METAL))

                ,dynamic_cast<Object*>(new Sphere(Vec3d(  2.0, 1.0, -1.25), 1.00, Colors::PURPLE, Materials::METAL))
                ,dynamic_cast<Object*>(new Sphere(Vec3d( -5.0, 2.0, -1.25), 2.00, Colors::GREEN,  Materials::PLASTIC))
                ,dynamic_cast<Object*>(new Sphere(Vec3d(  0.0, 2.0,  1.00), 2.00, Colors::YELLOW, Materials::METAL))
                ,dynamic_cast<Object*>(new Sphere(Vec3d(  2.9, 0.6, -2.40), 0.60, Colors::WHITE,  Materials::METAL))
                ,dynamic_cast<Object*>(new Sphere(Vec3d( -1.0, 3.0, -1.50), 1.00, Colors::WHITE,  Materials::GLOSSY))
                ,dynamic_cast<Object*>(new Sphere(Vec3d( -2.4, 1.0, -4.50), 1.00, Colors::WHITE,  Materials::TRANSLUCENT))
                ,dynamic_cast<Object*>(new Sphere(Vec3d(  4.0, 3.0,  4.00), 3.00, Colors::RED,    Materials::PLASTIC))
        );

        s = &Scenes::SCENE_0;
        break;

    case 1:
        Scenes::SCENE_1.add(
                dynamic_cast<Object*>(new Camera(Vec3d(0, 5, -20), Space::ORIGIN))

                ,dynamic_cast<Object*>(new Light(Vec3d(0, 5, -50), Colors::BLUE))

                ,dynamic_cast<Object*>(new Plane(Vec3d(0, 1,  0), 0.0, Colors::MAROON))
                ,dynamic_cast<Object*>(new Plane(Vec3d(0, 0,  -1), -20.0, Colors::GRAY))

                ,dynamic_cast<Object*>(new Sphere(Vec3d(-4, 1, -2), 2.0, Colors::MAGENTA))
                ,dynamic_cast<Object*>(new Sphere(Vec3d( 0, 1,  6), 2.0, Colors::YELLOW))
                ,dynamic_cast<Object*>(new Sphere(Vec3d( 4, 1, -2), 2.0, Colors::CYAN))
                ,dynamic_cast<Object*>(new Sphere(Vec3d( 0, 1,  0), 1.0, Colors::WHITE))
        );

        s = &Scenes::SCENE_1;
        break;

    default:
        s = &Scenes::SCENE_0;
        break;
    }

    s->max_depth()          = max_depth;
    s->nb_samples()         = samples_per_row_col;
    s->_output_img          = output_img;
    s->_mode                = mode;
    s->output_path()        = "./test.bmp";

    return s;
}
