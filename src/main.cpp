#include <iostream>

#include "Image/image.hpp"
#include "Scene/scene.hpp"
#include "Raytracer/ray.hpp"
#include "Object/Camera/camera.hpp"
#include "Object/Light/light.hpp"
#include "Object/Shape/sphere.hpp"
#include "Object/Shape/plane.hpp"
#include "Utils/save.hpp"

int main(int argc, char **argv)
{
    // Checking input arguments
    if (argc != 2)
    {
        std::cerr << "usage: " << argv[0] << " output_img" << std::endl;
        exit(1);
    }

    std::cout << "{o}----------------------------------------->\\" << std::endl;
    std::cout << "{o}---{-C-U-S-T-O-M---R-A-Y-T-R-A-C-E-R-}--->/"  << std::endl;
    std::cout << "{o}----------------------------------------->|"  << std::endl
                                                                   << std::endl;
    Image img(1600, 900);
    img.paint(Colors::BLACK);

    std::cout << "- Input scene: None" << std::endl;
    std::cout << "- Output file: " << argv[1] << std::endl;
    std::cout << "- Image aspect ratio : " << img.aspect_ratio() << std::endl;

    // Defining the scene objects
    Scene scene;

    Camera  c(Vec3d(0, 0, -4), Space::ORIGIN);
    Light   l1(Vec3d(-7, 10, -10), Colors::RED);
    Sphere  s1(Vec3d(0, 0, 1.25), 1.0, Colors::YELLOW);
    Sphere  s2(Vec3d(1.75, 0, 3.25), 1.0, Colors::PURPLE);
    Plane   p(Vec3d(0, 1, 0), 1.0, Colors::MAROON);

    scene.add(dynamic_cast<Object*>(&c),  dynamic_cast<Object*>(&l1),
              dynamic_cast<Object*>(&s1), dynamic_cast<Object*>(&s2),
              dynamic_cast<Object*>(&p));

    Vec3d xray = c.right();
    Vec3d yray = -1/img.aspect_ratio() * c.up();
    Vec3d zray = 1/tanf(M_PI * -120.0/360.0) * c.direction();

    bool collides = false;

    for (int j = 0; j < img.height(); ++j)
    {
        for (int i = 0; i < img.width(); ++i)
        {
            double  norm_i = (i+0.5)/img.width() - 0.5;
            double  norm_j = (j+0.5)/img.height() - 0.5;
            Vec3d   pixel_position = norm_i * xray + norm_j * yray + zray;

            Ray r(c.position(), pixel_position.normalized());

            double t, tmax = 20000;
            for (auto it = scene.shapes().begin(), st = scene.shapes().end();
                 it != st; ++it)
            {
                collides = ((*it)->intersect(r, t) && t < tmax) || collides;

                if (collides)
                {
                    img[i][j] = (*it)->color();
                    tmax = t;
                }
            }
        }
    }

    save2bmp(argv[1], img, 72);

    std::cout << std::endl
              << "{o}----------------------------------------->|" << std::endl;
    std::cout << "{o}----------------{-E-N-D-}---------------->\\"  << std::endl;
    std::cout << "{o}----------------------------------------->/"  << std::endl;

    return 0;
}
