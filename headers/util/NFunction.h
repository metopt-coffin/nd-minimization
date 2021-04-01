#pragma once

#include "Misc.h"
#include "QuadMatrix.h"

namespace util {

struct NFunction
{
    double operator()(const std::vector<double> x) const { return m_calculate(x); }

    double operator()(const Vector & vec) const
    {
        return m_a * vec * vec + m_b * vec + m_c;
    }

private:
    QuadMatrix m_a;
    Vector m_b;
    double m_c;
    CalculateNFunc m_calculate; // will probably be replaced with A, b, c parameters
};

} // namespace util