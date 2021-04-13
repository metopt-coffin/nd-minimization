#pragma once

#include "nd_methods/MinSearcher.h"

namespace min_nd {

struct ConjucateGrad : MinSearcher
{
    ConjucateGrad(double eps)
        : m_eps(eps)
    {}

protected:
    /*
     * Find n-dimensional function's minimum
     * using conjugate gradient method.
     */
    SearchRes find_min_impl() override;
    /*
     * Find n-dimensional function's minimum
     * using conjugate gradient method.
     * Outputs tracing information.
     */
    TracedSearchRes find_min_traced_impl() override;

protected:
    double m_eps; // required precision
};

} // namespace min_nd
