#ifndef MACROS_H
#define MACROS_H

#include <Eigen/Dense>
#include <cmath>

namespace math {
    const double PI = (4.0 * std::atan2(1.0, 1.0));

    template <typename T>
    constexpr T degreeToRadians(const T degree) {
        T radians = degree * PI / 180.f;
        return radians;
    }

    template <typename Derived>
    static inline bool is_nan(const Eigen::MatrixBase<Derived>& x) {
        return ((x.array() != x.array())).all();
    }

}  // namespace math

#endif
