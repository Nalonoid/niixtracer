#include <iostream>

#include "Image/image.hpp"
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
    img.paint(Colors::PURPLE);
    save2bmp(argv[1], img, 72);

    std::cout << std::endl
              << "{o}----------------------------------------->\\" << std::endl;
    std::cout << "{o}----------------{-E-N-D-}---------------->|"  << std::endl;
    std::cout << "{o}----------------------------------------->/"  << std::endl;

    Light   l1(Vec3d(-7, 10, -7), Colors::RED);
    Sphere  s1;
    Sphere  s2(Vec3d(3, 1, 1), 2.0, Colors::PURPLE);
    Plane   p(Space::YAXIS, -1.0, Colors::MAROON);

    return 0;
}
