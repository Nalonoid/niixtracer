#include <iostream>
#include <chrono>

#include <omp.h>

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
    if (argc < 3)
    {
        std::cerr << "usage: " << argv[0]
                  << " output_img"                      // Path to the output image
                     " scene_number"                    // ID of the scene to render
                     " [mode]"                          // Ray Tracer: rt / Monte Carlo Path Tracing : mcpt
                     " [super_sampling: 2, 4, 16,..]"   // Anti-aliasing: 4x, 16x, 256x,...
                     " [ref{le,ra}ction_depth]"         // Maximum number of bounces for ref{le,ra}ction
                  << std::endl;
        exit(1);
    }

    // Initialize arguments
    const char *output_path     { argv[1] };
    unsigned scene_number       { static_cast<unsigned>(atoi(argv[2])) };
    std::string mode            { argc >= 4 ? argv[3] : "rt" };
    unsigned nb_samples_row_col { static_cast<unsigned>(argc >= 5 ?
                                                           atoi(argv[4]) : 0) };
    unsigned scene_max_depth    { static_cast<unsigned>(argc >= 6 ?
                                                           atoi(argv[5]) : 0) };

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

    // Start the chrono!
    std::chrono::high_resolution_clock::time_point chrono_start =
            std::chrono::high_resolution_clock::now();

    // Defining the scene objects
    const Scene &scene  {
        init_scene(scene_number,
                   &img,
                   mode,
                   nb_samples_row_col,
                   scene_max_depth)
    };

    std::cout << std::endl
              << "{o}------------R-E-N-D-E-R-I-N-G------------>|" << std::endl;

    // Render the scene
    #pragma omp parallel for
    for (unsigned j = 0; j < img.height(); ++j)
        for (unsigned i = 0; i < img.width(); ++i)
            scene.render(i, j);

    // Stop the chrono!
    std::chrono::high_resolution_clock::time_point chrono_stop =
            std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>
            (chrono_stop - chrono_start).count();

    std::cout << std::endl << "- Rays count:\t" << Ray::number() << std::endl
                           << "- Elapsed time:\t" << duration/1000000.0
                           << "s" << std::endl;

    save2bmp(output_path, img, 72);

    std::cout << std::endl
              << "{o}----------------------------------------->|" << std::endl;
    std::cout << "{o}----------------{-E-N-D-}---------------->\\"  << std::endl;
    std::cout << "{o}----------------------------------------->/"  << std::endl;

    return 0;
}
