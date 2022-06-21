#ifndef MACROS_H
#define MACROS_H

#include <cmath>

namespace MathHelper
{
    template<typename T>
    constexpr T degreeToRadians(const T degree)
    {
        T radians = degree * M_PI / 180.f;
        return radians;
    }

    const double PI = (4.0 * std::atan2(1.0, 1.0));
} // namespace MathHelper

#endif
