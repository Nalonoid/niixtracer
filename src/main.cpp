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
#include "Utils/serializer.hpp"

int main(int argc, char **argv)
{
    // Checking input arguments
    if (argc != 2)
    {
        std::cerr << "usage: "
                  << argv[0] << " XML_scene_file_path" << std::endl;
        exit(1);
    }

    std::cout << "{o}----------------------------------------->\\" << std::endl;
    std::cout << "{o}---{-C-U-S-T-O-M---R-A-Y-T-R-A-C-E-R-}--->/"  << std::endl;
    std::cout << "{o}----------------------------------------->|"  << std::endl
              << std::endl;

    // Initialize arguments
    const char *scene_path      { argv[1] };
    Image *img = new Image(1600, 900);

    Serializer serializer;
    serializer.init(new Scene(img));
    Scene *scene { serializer.read_from_XML(scene_path) };

    // Display scene settings
    std::cout << "- Output file:\t" << scene->output_path() << std::endl;
    std::cout << "- Input scene:\t" << scene_path << std::endl;
    std::cout << "- Resolution:\t" << img->width()
              << "x" << img->height() << std::endl;

    std::cout << "- Aspect ratio:\t" << img->aspect_ratio() << std::endl;
    std::cout << "- Rendering mode:\t" << scene->mode() << std::endl;
    std::cout << "- Max ray bounces:\t" << scene->max_depth() << std::endl;
    std::cout << "- Sampling:\t" << scene->nb_samples() << "x" << std::endl;

    std::cout << std::endl
              << "{o}------------R-E-N-D-E-R-I-N-G------------>|" << std::endl;

    // Start the chrono!
    std::chrono::high_resolution_clock::time_point chrono_start =
            std::chrono::high_resolution_clock::now();

    // Render the scene
    #pragma omp parallel for
    for (unsigned j = 0; j < img->height(); ++j)
        for (unsigned i = 0; i < img->width(); ++i)
            scene->render(i, j);

    // Stop the chrono!
    std::chrono::high_resolution_clock::time_point chrono_stop =
            std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>
            (chrono_stop - chrono_start).count();

    // Save the image to the specified output file
    save2bmp(scene->output_path().data(), img, 72);

    // Display some information about the rendering
    std::cout << std::endl << "- Rays count:\t" << Ray::number() << std::endl
                           << "- Elapsed time:\t" << duration/1000000.0
                           << "s" << std::endl;

    std::cout << std::endl
              << "{o}----------------------------------------->|" << std::endl;
    std::cout << "{o}----------------{-E-N-D-}---------------->\\"  << std::endl;
    std::cout << "{o}----------------------------------------->/"  << std::endl;

    return 0;
}
