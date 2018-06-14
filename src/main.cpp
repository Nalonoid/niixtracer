#include <iostream>
#include <chrono>

#include "Image/image.hpp"
#include "Scene/scene.hpp"
#include "Raytracer/ray.hpp"
#include "Raytracer/raytracer.hpp"
#include "Raytracer/pathtracer.hpp"
#include "Raytracer/spectral_pathtracer.hpp"
#include "Utils/save.hpp"
#include "Utils/serializer.hpp"

#include "Raytracer/spectrum.hpp"

int main(int argc, char **argv)
{
    // Checking input arguments
    if (argc != 2)  // != 4 if you're using render.sh 2 otherwise
    {
        std::cerr << "usage: "
                  << argv[0] << " XML_scene_file_path" << std::endl;
        return 1;
    }

    // Initialize arguments
    const char *scene_path { argv[1] };

    // Check whether the scene file exists. If not, exit the program
    if (!fopen(scene_path, "r"))
    {
        std::cerr << "error: the input scene file does not exist : "
                  << scene_path << std::endl;
        return 1;
    }

    Image *img = new Image();

    // Read scene from the XML file
    Serializer serializer;
    serializer.init(new Scene(img));
    Scene *scene { serializer.read_from_XML(scene_path) };

    /* To be used with render.sh if you want to render the same scene at
     * different number of samples */
//    scene->nb_samples() = sqrt(std::stoi(argv[2]));
//    scene->output_path() = std::string(argv[3]);

    Renderer *renderer {
        scene->mode() == "rt" ?
                    (Renderer*) new Raytracer(scene)  :
        scene->mode() == "mcpt" ?
                    (Renderer*) new Pathtracer(scene) :
        scene->mode() == "spectral_mcpt" ?
                    (Renderer*) new SpectralPathtracer(scene) : nullptr
    };

    if (!renderer)
    {
        std::cerr << "error: the renderer is not valid : "
                  << scene->mode() << std::endl;
        return 1;
    }

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

    std::time_t start { std::chrono::system_clock::to_time_t(chrono_start) };
    std::cout << std::endl << "- Rendering start:\t" << std::ctime(&start)
              << std::flush;

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

    std::time_t end { std::chrono::system_clock::to_time_t(chrono_stop) };
    std::cout << "- Rendering end:\t" << std::ctime(&end);

    std::cout << "- Elapsed time:\t" << duration/1000000.0 << "s" << std::endl
              << "- Rays count:\t" << Ray::number() << "+" << Ray::over_number()
              << std::endl;

    std::cout << std::endl
              << "{o}----------------------------------------->|"  << std::endl;
    std::cout << "{o}----------------{-E-N-D-}---------------->\\" << std::endl;
    std::cout << "{o}----------------------------------------->/"  << std::endl;

    return 0;
}
