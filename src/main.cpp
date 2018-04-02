#include <iostream>

#include "Image/image.hpp"
#include "Raytracer/ray.hpp"
#include "Camera/camera.hpp"
#include "Light/light.hpp"
#include "Shapes/sphere.hpp"
#include "Shapes/plane.hpp"
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
    Image img(1920, 1080);

    std::cout << "- Input scene: None" << std::endl;
    std::cout << "- Output file: " << argv[1] << std::endl;
    std::cout << "- Image aspect ratio : " << img.aspect_ratio() << std::endl;

    img.paint(Colors::BLACK);

    Camera  c(Vec3d(0, 0, -4), Space::ORIGIN);
    //Light   l1(Vec3d(-7, 10, -10), Colors::RED);
    Sphere  s1(Vec3d(0, 0, 1.25), 1.0, Colors::YELLOW);
    Sphere  s2(Vec3d(1.75, 0, 3.25), 1.0, Colors::PURPLE);
    Plane   p(Vec3d(0, 1, 0), 1.0, Colors::MAROON);

    Vec3d xray = c.right();
    Vec3d yray = -1/img.aspect_ratio() * c.up();
    Vec3d zray = 1/tanf(M_PI * -120.0/360.0) * c.direction();
    Vec3d d;

    for (int j = 0; j < img.height(); ++j)
    {
        for (int i = 0; i < img.width(); ++i)
        {
            double  norm_i = (i+0.5)/img.width() - 0.5;
            double  norm_j = (j+0.5)/img.height() - 0.5;
            Vec3d   pixel_position = norm_i * xray + norm_j * yray + zray;

            Ray r(c.position(), pixel_position.normalized());

            double t = 20000;

            if (p.intersect(r, t))
                img[i][j] = p.color();

            if (s2.intersect(r, t))
                img[i][j] = s2.color();

            if (s1.intersect(r, t))
                img[i][j] = s1.color();
        }
    }

    save2bmp(argv[1], img, 72);

    std::cout << std::endl
              << "{o}----------------------------------------->\\" << std::endl;
    std::cout << "{o}----------------{-E-N-D-}---------------->|"  << std::endl;
    std::cout << "{o}----------------------------------------->/"  << std::endl;

    return 0;
}
