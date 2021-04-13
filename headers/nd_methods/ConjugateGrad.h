#pragma once

#include "nd_methods/MinSearcher.h"

namespace min_nd {

struct ConjucateGrad : MinSearcher
{
    ConjucateGrad(double eps)
        : m_eps(eps)
    {}

protected:
    SearchRes find_min_impl() override;
    TracedSearchRes find_min_traced_impl() override;

protected:
    double m_eps;
};

} // namespace min_nd
