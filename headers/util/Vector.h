#pragma once

#include <numeric>
#include <vector>

namespace util {

struct QuadMatrix;

struct Vector
{
    friend struct QuadMatrix;
    explicit Vector(std::vector<double> vec)
        : m_data(std::move(vec))
    {}

    double operator*(const Vector & rhs) const noexcept
    {
        return std::transform_reduce(m_data.begin(), m_data.end(), rhs.m_data.begin(), 0.,
            std::plus<double>{}, std::multiplies<double>{});
    }

    std::size_t dims() const noexcept { return m_data.size(); }
private:
    std::vector<double> m_data;
};
} // namespace util
