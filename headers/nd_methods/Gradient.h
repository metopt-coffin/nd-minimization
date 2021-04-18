#pragma once

#include "nd_methods/ConjugateGrad.h"
#include "nd_methods/MinSearcher.h"

namespace min_nd {

struct Gradient : ConjucateGrad
{
    Gradient(double eps, double max_step)
        : ConjucateGrad(eps)
        , m_alpha(max_step)
    {}

protected:
    /*
     * Find n-dimensional function's minimum
     * using gradient descent method.
     */
    SearchRes find_min_impl() override;
    /*
     * Find n-dimensional function's minimum
     * using gradient descent method.
     * Outputs tracing information.
     */
    TracedSearchRes find_min_traced_impl() override;

protected:
    double m_alpha; // max step
};

} // namespace min_nd
