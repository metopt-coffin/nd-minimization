#pragma once

#include "Misc.h"
#include "DiagMatrix.h"

#include "util/Vector.h"

namespace util {

struct NFunction
{
    NFunction(DiagMatrix a, Vector b, double c, double eigenvalue)
        : m_a(std::move(a))
        , m_b(std::move(b))
        , m_c(c)
        , max_eigenvalue(eigenvalue)
    {}

    double operator()(const std::vector<double> x) const { return m_calculate(x); }

    double operator()(const Vector & vec) const
    {
        return m_a * vec * vec * 0.5 + m_b * vec + m_c;
    }

    Vector grad(const Vector & vec) const { return m_a * vec + m_b; }

    std::size_t dims() const noexcept { return m_a.dims(); }

    const DiagMatrix & a() const noexcept { return m_a; }
    const Vector & b() const noexcept { return m_b; }
    double c() const noexcept { return m_c; }
    double eigenvalue() const noexcept { return max_eigenvalue; }

private:
    DiagMatrix m_a;
    Vector m_b;
    double m_c;
    double max_eigenvalue;
    CalculateNFunc m_calculate; // will probably be replaced with A, b, c parameters
};

} // namespace util