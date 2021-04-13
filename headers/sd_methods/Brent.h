#pragma once

#include "MinSearcher.h"

#include "util/Function.h"
#include "util/ReplayData.h"

#include <cmath>

namespace min1d {

struct Brent : public MinSearcher
{
    static inline const double TAU = (3 - sqrt(5)) / 2; // coefficient as in gloden ratio

    Brent(double eps)
        : m_eps(eps)
    {}


    std::string_view method_name() const noexcept override { return "Brent"; }

    void change_parameters(double new_eps) noexcept { m_eps = new_eps; }

protected:
    /*
     * Find unimodal function's minimum
     * using Brent's method.
     */
    SearchRes find_min_impl() noexcept override;

    /*
     * Find unimodal function's minimum
     * using Brent's method.
     * Outputs tracing information.
     */
    TracedSearchRes find_min_tracked_impl() noexcept override;

    double m_eps; // required accuracy
};

} // namespace min1d
