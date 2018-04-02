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
    std::cout << "- Input scene: None" << std::endl;
    std::cout << "- Output file: " << argv[1] << std::endl;

    Image img(1920, 1080);
    img.paint(Colors::GRAY);

    Camera  c(Vec3d(3, 1.5, -4), Space::ORIGIN);
    Light   l1(Vec3d(-7, 10, -10), Colors::RED);
    Sphere  s1(Vec3d(1.75, -0.25, 0), 1.0, Colors::LIME);
    //Sphere  s2(Vec3d(img.width()/2, img.height()/2, 0), 50.0, Colors::PURPLE);
    Plane   p(Vec3d(0, 1, -1), -1.0, Colors::MAROON);

    double xamnt, yamnt;

    for (unsigned j = 0; j < img.height(); ++j)
    {
        for (unsigned i = 0; i < img.width(); ++i)
        {
            if (img.aspect_ratio() > 1) // width > height
            {
                xamnt = (i+0.5)/img.width()*img.aspect_ratio() -
                        ((img.width()-img.height())/img.height())/2;
                yamnt = ((img.height() - j) + 0.5)/img.height();
            }
            else
            {
                if (img.aspect_ratio() < 1)
                {
                    xamnt = (i + 0.5)/img.width();
                    yamnt = (((img.height() - j) + 0.5) /
                             img.height()) / img.aspect_ratio() -
                            (((img.height() - img.width())/img.width())/2);
                }
                else
                {
                    // square image
                    xamnt = (i + 0.5) / img.width();
                    yamnt = ((img.height() - j) + 0.5)/img.height();
                }
            }

            Ray r(c.position(),
                  (c.direction() +
                    (c.right() * (xamnt-0.5) + c.down() * (yamnt-0.5))).normalized());
            double t = 20000;

            if (p.intersect(r, t) && t < 520.0)
                img[i][j] = p.color();

            if (s1.intersect(r, t))
                img[i][j] = s1.color();

//            if (s2.intersect(r, t))
//                img[i][j] = s2.color();
        }
    }

    save2bmp(argv[1], img, 72);

    std::cout << std::endl
              << "{o}----------------------------------------->\\" << std::endl;
    std::cout << "{o}----------------{-E-N-D-}---------------->|"  << std::endl;
    std::cout << "{o}----------------------------------------->/"  << std::endl;

    return 0;
}
