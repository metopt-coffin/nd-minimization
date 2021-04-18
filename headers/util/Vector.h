#pragma once

#include <algorithm>
#include <cassert>
#include <functional>
#include <numeric>
#include <ostream>
#include <vector>

namespace util {

struct QuadMatrix;

struct Vector
{
    friend struct DiagMatrix;
    friend struct QuadMatrix;

    explicit Vector(std::size_t dims)
        : m_data(dims, 0.)
    {}

    explicit Vector(std::vector<double> vec)
        : m_data(std::move(vec))
    {}

    double operator*(const Vector & rhs) const noexcept
    {
        return std::transform_reduce(m_data.begin(), m_data.end(), rhs.m_data.begin(), 0., std::plus<double>{}, std::multiplies<double>{});
    }

    Vector operator*(double scalar) const noexcept
    {
        std::vector res(m_data);
        std::for_each(res.begin(), res.end(), [scalar](double & x) { x *= scalar; });
        return Vector{std::move(res)};
    }

    friend Vector operator*(double scalar, const Vector & vec) noexcept { return vec * scalar; }

    Vector operator+(const Vector & rhs) const noexcept { return plus_minus(rhs, std::plus<double>{}); }

    Vector operator-(const Vector & rhs) const noexcept { return plus_minus(rhs, std::minus<double>{}); }

    std::size_t dims() const noexcept { return m_data.size(); }

    double length_pow2() const noexcept { return *this * *this; }

    friend std::ostream & operator<<(std::ostream & out, const Vector & vec)
    {
        out << "{";
        std::for_each(vec.m_data.begin(), vec.m_data.end(), [&out, first(true)](double x) mutable {
            if (!first) {
                out << ", ";
            } else {
                first = false;
            }
            out << x;
        });
        out << "}";
        return out;
    }

private:
    template <class Func>
    Vector plus_minus(const Vector & rhs, Func && to_apply) const noexcept
    {
        assert(dims() == rhs.dims() && "Vector::plus_minus dimension mismatch");
        std::vector<double> res(dims());
        std::transform(m_data.begin(), m_data.end(), rhs.m_data.begin(), res.begin(), std::forward<Func>(to_apply));
        return Vector{std::move(res)};
    }

private:
    std::vector<double> m_data;
};
} // namespace util
