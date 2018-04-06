#include <iostream>

#include "Image/image.hpp"
#include "Scene/scene.hpp"
#include "Scene/scene_examples.inl"
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

    // Display image characteristics
    std::cout << "- Input scene:\t" << "None" << std::endl;
    std::cout << "- Output file:\t" << argv[1] << std::endl;
    std::cout << "- Resolution:\t" << img.width()
              << "x" << img.height() << std::endl;

    std::cout << "- Aspect ratio:\t" << img.aspect_ratio() << std::endl;

    // Defining the scene objects
    const Scene &scene = populate_scene(0);
    Camera &c = scene.camera(0);

    Vec3d left  = c.left();
    Vec3d up    = 1/img.aspect_ratio() * c.up();
    Vec3d front = 1/tanf(PI * 120.0/360.0) * c.direction();

    bool collides;

    std::cout << std::endl
              << "{o}------------R-E-N-D-E-R-I-N-G------------>|" << std::endl;

    for (int j = 0; j < img.height(); ++j)
    {
        for (int i = 0; i < img.width(); ++i)
        {
            collides = false;
            double t = 20000;

            double  norm_i = (i+0.5)/img.width() - 0.5;
            double  norm_j = (j+0.5)/img.height() - 0.5;
            Vec3d   pixel_position = (norm_i * left) + (norm_j * up) + front;
            Ray     r(c.position(), pixel_position.normalized());

            for (auto it = scene.shapes().begin(), st = scene.shapes().end(); it != st; ++it)
                collides = (*it)->intersect(r, t) || collides;

            if (collides)
                img[i][j] = scene.compute_color(r);
        }
    }

    std::cout << std::endl << "- Rays count:\t" << Ray::number() << std::endl;

    save2bmp(argv[1], img, 72);

    std::cout << std::endl
              << "{o}----------------------------------------->|" << std::endl;
    std::cout << "{o}----------------{-E-N-D-}---------------->\\"  << std::endl;
    std::cout << "{o}----------------------------------------->/"  << std::endl;

    return 0;
}
