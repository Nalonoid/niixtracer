#include "color.hpp"
#include "Utils/utils.hpp"

double Color::brightness() const
{
    return (r() + g() + b())/(3*255);
}

const Color& Color::scale(float p)
{
    r() = (int) clamp(p * r(), 0.f, 255.f);
    g() = (int) clamp(p * g(), 0.f, 255.f);
    b() = (int) clamp(p * b(), 0.f, 255.f);

    return *this;
}

namespace Colors
{

Color average(const Color &c1, const Color &c2)
{
    return Color((c1.r() + c2.r())/2,
                 (c1.g() + c2.g())/2,
                 (c1.b() + c2.b())/2);
}

}
