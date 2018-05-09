#include <iostream>
#include <chrono>

#include <omp.h>

#include "Image/image.hpp"
#include "Scene/scene.hpp"
#include "Scene/scene_examples.inl"
#include "Raytracer/ray.hpp"
#include "Raytracer/raytracer.hpp"
#include "Raytracer/pathtracer.hpp"
#include "Object/Camera/camera.hpp"
#include "Object/Light/light.hpp"
#include "Object/Shape/sphere.hpp"
#include "Object/Shape/plane.hpp"
#include "Utils/save.hpp"
#include "Utils/serializer.hpp"
#include "Utils/sampler.hpp"

int main(int argc, char **argv)
{    
    // Checking input arguments
    if (argc != 2)
    {
        std::cerr << "usage: "
                  << argv[0] << " XML_scene_file_path" << std::endl;
        return 1;
    }

    // Initialize arguments
    const char *scene_path { argv[1] };

    halton_sampler1.number(0, 2);
    halton_sampler2.number(0, 3);

    // Check whether the scene file exists. If not, exit the program
    if (!fopen(scene_path, "r"))
    {
        std::cerr << "error: the input scene file does not exist" << std::endl;
        return 1;
    }

    Image *img = new Image(1600, 900);

    // Read scene from the XML file
    Serializer serializer;
    serializer.init(new Scene(img));
    Scene *scene { serializer.read_from_XML(scene_path) };

    Renderer *renderer { scene->mode() == "rt" ?
        (Renderer*) new Raytracer(scene) : (Renderer*) new Pathtracer(scene) };

    // Display scene settings
    std::cout << "{o}----------------------------------------->\\" << std::endl;
    std::cout << "{o}---{-C-U-S-T-O-M---R-A-Y-T-R-A-C-E-R-}--->/"  << std::endl;
    std::cout << "{o}----------------------------------------->|"  << std::endl
              << std::endl;

    std::cout << "- Output file:\t" << scene->output_path() << std::endl;
    std::cout << "- Input scene:\t" << scene_path << std::endl;
    std::cout << "- Resolution:\t" << img->width()
              << "x" << img->height() << std::endl;

    std::cout << "- Aspect ratio:\t" << img->aspect_ratio() << std::endl;
    std::cout << "- Rendering mode:\t" << scene->mode() << std::endl;
    std::cout << "- Max ray bounces:\t" << scene->max_depth() << std::endl;
    std::cout << "- Sampling:\t" << scene->nb_samples()*scene->nb_samples()
              << "x" << std::endl;

    std::cout << std::endl
              << "{o}------------R-E-N-D-E-R-I-N-G------------>|" << std::endl;

    // Start the chrono!
    std::chrono::high_resolution_clock::time_point chrono_start =
            std::chrono::high_resolution_clock::now();

    // Render the scene
    renderer->render_scene();

    // Stop the chrono!
    std::chrono::high_resolution_clock::time_point chrono_stop =
            std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>
            (chrono_stop - chrono_start).count();

    // Save the image to the specified output file
    save2bmp(scene->output_path().data(), img, 72);

    // Clean data structures
    delete renderer;

    // Display some information about the rendering
    std::cout << std::endl << "- Rays count:\t" << Ray::number() << "+"
              << Ray::over_number() << std::endl << "- Elapsed time:\t"
              << duration/1000000.0 << "s" << std::endl;

    std::cout << std::endl
              << "{o}----------------------------------------->|" << std::endl;
    std::cout << "{o}----------------{-E-N-D-}---------------->\\"  << std::endl;
    std::cout << "{o}----------------------------------------->/"  << std::endl;

    return 0;
}
