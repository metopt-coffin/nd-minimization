#pragma once

#include "nd_methods/MinSearcher.h"

namespace min_nd {

struct Gradient : MinSearcher
{
    Gradient(double eps, double max_step)
        : m_eps(eps)
        , m_alpha(max_step)
    {}
protected:
    SearchRes find_min_impl() override;
    TracedSearchRes find_min_traced_impl() override;

private:
    double m_eps;
    double m_alpha;
};

}
