#pragma once

#include "RectangledVector.h"
#include "Vector.h"

#include <algorithm>
#include <cassert>
#include <vector>

namespace util {

struct QuadMatrix
{
    using DataHolder = RectangledVector<double>;

    QuadMatrix(std::size_t dims)
        : m_data(dims, dims)
    {}
    QuadMatrix(std::vector<std::vector<double>> data)
        : m_data(std::move(data))
    {
    }

    DataHolder::RowView operator[](std::size_t idx) noexcept { return m_data[idx]; }

    Vector operator*(const Vector & rhs) const noexcept
    {
        assert(rhs.dims() == m_data.rows() && "Matrix by Vector dim mismatch");

        std::vector<double> res(rhs.dims(), 0);
        std::transform(m_data.begin(), m_data.end(), res.begin(), [&rhs](auto row) {
            return std::transform_reduce(row.begin(), row.end(), rhs.m_data.begin(), 0., std::plus<double>{}, std::multiplies<double>{});
        });
        return Vector(std::move(res));
    }

    std::size_t dims() const noexcept { return m_data.cols(); }

private:
    DataHolder m_data;
};

} // namespace util
