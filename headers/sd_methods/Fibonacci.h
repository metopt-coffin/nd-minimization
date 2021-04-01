#pragma once

#include "MinSearcher.h"

#include "util/Function.h"
#include "util/ReplayData.h"

namespace min1d {

struct Fibonacci : public MinSearcher
{
    Fibonacci(double eps)
        : m_eps(eps)
    {}

    std::string_view method_name() const noexcept override { return "Fibonacci"; }

    void change_parameters(double new_eps) noexcept { m_eps = new_eps; }

protected:
    /*
      * Find unimodal function's minimum
      * using fibonacci method.
      */
    double find_min_impl() noexcept override;

    /*
     * Find unimodal function's minimum
     * using fibonacci method.
     * Outputs tracing information.
     */
    double find_min_tracked_impl() noexcept override;

    double m_eps; // required accuracy
};

} // namespace min1d