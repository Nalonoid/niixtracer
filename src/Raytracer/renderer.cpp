#include <iomanip>

#include "renderer.hpp"
#include "ray.hpp"

#include "Image/color.hpp"
#include "Image/image.hpp"
#include "Scene/scene.hpp"
#include "Object/Shape/shape.hpp"
#include "Object/Camera/camera.hpp"
#include "Utils/sampler.hpp"

#include "spectral_pathtracer.hpp"

Renderer::Renderer(Scene *scene) : _scene(scene) {}

Renderer::~Renderer() {}

std::chrono::high_resolution_clock::time_point start_timer()
{
    // Start the chrono!
    std::chrono::high_resolution_clock::time_point chrono_start =
            std::chrono::high_resolution_clock::now();

    std::time_t start { std::chrono::system_clock::to_time_t(chrono_start) };
    std::cout << std::setw(20) << std::setfill(' ') << std::endl
              << "- Rendering start:" << std::ctime(&start);

    return chrono_start;
}

long int update_timer(
        const std::chrono::high_resolution_clock::time_point& start_time,
        unsigned rendered_pixels, unsigned total_pixels)
{
    std::chrono::high_resolution_clock::time_point current_time =
            std::chrono::high_resolution_clock::now();

    long int duration = std::chrono::duration_cast<std::chrono::microseconds>
            (current_time - start_time).count();

    std::cout << std::left << std::setw(21) << std::setfill(' ')
              << "\r- Elapsed time:"
              << std::left << std::setw(20) << std::setfill(' ')
              << duration/1000000.0
              << "| " << rendered_pixels*100/total_pixels << "%" << std::flush;

    return duration;
}

void stop_timer()
{
    // Stop the chrono!
    std::chrono::high_resolution_clock::time_point chrono_stop =
            std::chrono::high_resolution_clock::now();

    std::time_t end { std::chrono::system_clock::to_time_t(chrono_stop) };
    std::cout << std::setw(20) << std::setfill(' ') << std::endl
              << "- Rendering end:" << std::ctime(&end);
}

void Renderer::render_scene()
{
    Image *img { _scene->_output_img };
    double norm_i, norm_j;
    Vec3d towards_pixel;

    const Camera &c { camera(0) };

    unsigned nb_samples     { _scene->nb_samples()      };
    unsigned total_samples  { nb_samples*nb_samples     };
    const double range      { 1.0/(double)nb_samples    };

    Uniform<std::uniform_real_distribution, double> sampler(0.0, range);

    std::chrono::high_resolution_clock::time_point timer { start_timer() };
    unsigned rendered_pixels { 0 };

    #pragma omp parallel for private(norm_i, norm_j, towards_pixel, uniform_sampler_double, sampler) schedule(dynamic)
    for (unsigned j = 0; j < img->height(); ++j)
    {
        for (unsigned i = 0; i < img->width(); ++i)
        {
            Color avg_color { Colors::BLACK };

            for (double xsp = 0; xsp < 1.0; xsp += range)
            {
                for (double ysp = 0; ysp < 1.0; ysp += range)
                {
                    /* Using a stratified sampling instead of an uniform sampling
                     * TO-DO: use Poisson-disk / Sobol sequence instead ?
                     * See: Low-discrepancy sequence */
                    double u { total_samples > 1 ? sampler.sample() : 0.5 };
                    double v { total_samples > 1 ? sampler.sample() : 0.5 };

                    norm_i        = (i+xsp+u)/img->width()  - 0.5;
                    norm_j        = (j+ysp+v)/img->height() - 0.5;
                    towards_pixel = (norm_i * c.left()) + (norm_j * c.up())
                            + c.direction();

                    Ray ray(c.position(), towards_pixel.normalized());
                    avg_color += launch(ray);
                }
            }

            avg_color = Colors::average(
                        avg_color, nb_samples*nb_samples).clamp();

            (*img)[i][j] = avg_color;

            #pragma omp critical
            update_timer(timer, ++rendered_pixels, img->height()*img->width());
        }
    }

    stop_timer();
}

#include "Object/Shape/sphere.hpp"
Color Renderer::launch(Ray &ray)
{
    bool collides { false };

    if (depth_recursion_over(ray))
        return Colors::BLACK;

    for (auto itr = shapes().begin(); itr < shapes().end(); itr++)
        collides = (*itr)->intersect(ray) || collides;

    if (!collides)
        return Colors::BLACK;

    return compute_color(ray);
}

const std::vector<Shape*>& Renderer::shapes() const
{
    return _scene->shapes();
}

const std::vector<Light*>& Renderer::lights() const
{
    return _scene->lights();
}

const std::vector<Camera*>& Renderer::cameras() const
{
    return _scene->cameras();
}

Shape& Renderer::shape(unsigned i) const
{
    return *(shapes()[i]);
}

Light& Renderer::light(unsigned i) const
{
    return *(lights()[i]);
}

Camera& Renderer::camera(unsigned i) const
{
    return *(cameras()[i]);
}
