#include "color.hpp"
#include "Utils/utils.hpp"

double Color::brightness() const
{
    return (r() + g() + b())/3;
}

const Color& Color::scale(float p)
{
    r() = p * r();
    g() = p * g();
    b() = p * b();

    clamp();

    return *this;
}

const Color& Color::clamp()
{
    r() = ::clamp(r(), 0, 1);
    g() = ::clamp(g(), 0, 1);
    b() = ::clamp(b(), 0, 1);

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

// Operators
Color operator+(const Color &c1, const Color &c2)
{
    return Color(c1.rgb + c2.rgb);
}

Color operator-(const Color &c1, const Color &c2)
{
    return Color(c1.rgb - c2.rgb);
}

Color operator*(const Color &c, const double scalar)
{
    return Color(c.rgb * scalar);
}

Color operator*(const double scalar, const Color &c)
{
    return Color(c.rgb * scalar);
}

Color operator*(const Color &c1, const Color &c2)
{
    return Color(c1.r() * c2.r(),
                 c1.g() * c2.g(),
                 c1.b() * c2.b());
}
