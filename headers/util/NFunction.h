#pragma once

#include "Misc.h"
#include "QuadMatrix.h"
#include "util/Vector.h"

namespace util {

struct NFunction
{
    NFunction(QuadMatrix a, Vector b, double c)
        : m_a(std::move(a))
        , m_b(std::move(b))
        , m_c(c)
    {}

    double operator()(const std::vector<double> x) const { return m_calculate(x); }

    double operator()(const Vector & vec) const
    {
        return m_a * vec * vec * 0.5 + m_b * vec + m_c;
    }

    Vector grad(const Vector & vec) const { return m_a * vec + m_b; }

    std::size_t dims() const noexcept { return m_a.dims(); }

private:
    QuadMatrix m_a;
    Vector m_b;
    double m_c;
    CalculateNFunc m_calculate; // will probably be replaced with A, b, c parameters
};

} // namespace util