#pragma once

#include "util/Vector.h"

#include <cassert>
#include <random>
#include <vector>

namespace util {

struct DiagMatrix
{
    explicit DiagMatrix(std::size_t dims, double min = 0., double max = 0.)
        : m_data(dims, 0.)
    {
        assert(dims > 0 && "zero-dimensional matrix is strange and not supported");
        std::mt19937 rand_engn(std::random_device{}());
        std::uniform_real_distribution dst(min, max);

        for (auto & el : m_data) {
            el = dst(rand_engn);
        }
        m_data.front() = min;
        m_data.back() = max;
    }

    explicit DiagMatrix(std::vector<double> diag)
        : m_data(std::move(diag))
    {}

    Vector operator*(const Vector & vec) const
    {
        assert(dims() == vec.dims() && "Matrix by Vector dim mismatch");
        std::vector res(m_data);
        auto other_it = vec.m_data.begin();
        for (auto & el : res) {
            el *= *other_it++;
        }
        return Vector(std::move(res));
    }

    explicit operator Vector() const
    {
        return Vector(m_data);
    }

    std::size_t dims() const noexcept { return m_data.size(); }

private:
    std::vector<double> m_data;
};

inline std::ostream & operator<<(std::ostream & out, const DiagMatrix & mtx)
{
    return out << Vector(mtx);
}

}
