#include "renderer.hpp"
#include "ray.hpp"

#include "Image/color.hpp"
#include "Image/image.hpp"
#include "Scene/scene.hpp"
#include "Object/Shape/shape.hpp"
#include "Object/Camera/camera.hpp"
#include "Utils/sampler.hpp"

Renderer::Renderer(Scene *scene) : _scene(scene) {}

void Renderer::render()
{
    Image *img { _scene->_output_img };
    double norm_i, norm_j;
    Vec3d towards_pixel;
    Color avg_color;

    const Camera &c { camera(0) };

    unsigned nb_samples = _scene->nb_samples();
    const double range { 1.0/(double)nb_samples};
    Uniform sampler(0.0, range);

    #pragma omp parallel for private(norm_i, norm_j, towards_pixel, avg_color) schedule(dynamic)
    for (unsigned j = 0; j < img->height(); ++j)
    {
        for (unsigned i = 0; i < img->width(); ++i)
        {
            avg_color = Colors::BLACK;

            for (double x = 0; x < 1.0; x += range)
            {
                for (double y = 0; y < 1.0; y += range)
                {
                    /* Using a stratified sampling instead of an uniform sampling
                     * TO-DO: use Poisson-disk / Sobol sequence instead ?
                     * See: Low-discrepancy sequence */
                    double u { nb_samples > 1 ? sampler.sample() : 0.5 };
                    double v { nb_samples > 1 ? sampler.sample() : 0.5 };

                    norm_i        = (i+x+u)/img->width()  - 0.5;
                    norm_j        = (j+y+v)/img->height() - 0.5;
                    towards_pixel = (norm_i * c.left()) + (norm_j * c.up())
                            + c.direction();

                    Ray r(c.position(), towards_pixel);

                    avg_color += launch(r);
                }
            }

            (*img)[i][j] =
                    Colors::average(avg_color, nb_samples*nb_samples).clamp();
        }
    }
}

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
