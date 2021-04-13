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
    SearchRes find_min_impl() override;
    TracedSearchRes find_min_traced_impl() override;

protected:
    double m_alpha;
};

} // namespace min_nd
