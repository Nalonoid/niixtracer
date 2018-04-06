#include "Scene/scene.hpp"
#include "Object/Camera/camera.hpp"
#include "Object/Light/light.hpp"
#include "Object/Shape/sphere.hpp"
#include "Object/Shape/plane.hpp"

namespace Scenes
{

Scene SCENE_0;
Scene SCENE_1;

}

const Scene& populate_scene(unsigned index)
{
    switch (index) {
    case 0:
        Scenes::SCENE_0.add(
                dynamic_cast<Object*>(new Camera(Vec3d(0, 3, -10), Space::ORIGIN)),

                dynamic_cast<Object*>(new Light(Vec3d(0, 3, -5), Colors::WHITE)),

                dynamic_cast<Object*>(new Sphere(Vec3d(0, 2,  1),    2.0, Colors::YELLOW)),
                dynamic_cast<Object*>(new Sphere(Vec3d(2, 1, -1.25), 1.0, Colors::PURPLE)),

                dynamic_cast<Object*>(new Plane(Vec3d(0, 1,  0),  0.0, Colors::MAROON))
                ,dynamic_cast<Object*>(new Plane(Vec3d(0, 0, -1), -10.0, Colors::BLUE))
        );

        return Scenes::SCENE_0;
        break;
    case 1:
        Scenes::SCENE_1.add(
                dynamic_cast<Object*>(new Camera(Vec3d(0, 5, -20), Space::ORIGIN)),

                dynamic_cast<Object*>(new Light(Vec3d(0, 5, -50), Colors::BLUE)),

                dynamic_cast<Object*>(new Sphere(Vec3d(-4, 1, -2), 2.0, Colors::MAGENTA)),
                dynamic_cast<Object*>(new Sphere(Vec3d( 0, 1,  6), 2.0, Colors::YELLOW)),
                dynamic_cast<Object*>(new Sphere(Vec3d( 4, 1, -2), 2.0, Colors::CYAN)),
                dynamic_cast<Object*>(new Sphere(Vec3d( 0, 1,  0), 1.0, Colors::WHITE)),

                dynamic_cast<Object*>(new Plane(Vec3d(0, 0,  1), 20.0, Colors::GRAY)),
                dynamic_cast<Object*>(new Plane(Vec3d(0, 1,  0), 0.0, Colors::MAROON))
        );

        return Scenes::SCENE_1;
        break;
    default:
        return Scenes::SCENE_0;
        break;
    }
}
