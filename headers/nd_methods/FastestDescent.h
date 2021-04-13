#pragma once

#include "MinSearcher.h"
#include "nd_methods/Gradient.h"
#include "sd_methods/MinSearcher.h"

#include "util/Function.h"
#include "util/Vector.h"

namespace min_nd {

struct FastestDescent : Gradient    // derived from Gradient to reuse variables
{
    FastestDescent(double eps, double max_step, min1d::MinSearcher & sd_searcher)
        : Gradient(eps, max_step)
        , m_sd_searcher(&sd_searcher)
    {}

public:
    /*
     * Set current one dimensional minimization method.
     */
    void set_sd_searcher(min1d::MinSearcher & sd_searcher) { m_sd_searcher = &sd_searcher; }

protected:
    /*
     * Find n-dimensional function's minimum 
     * using fastest descent method.
     */
    SearchRes find_min_impl() override;
    /*
     * Find n-dimensional function's minimum
     * using fastest descent method.
     * Outputs tracing information.
     */
    TracedSearchRes find_min_traced_impl() override;

protected:
    /*
     * Solve the one dimensional minimization problem.
     */
    min1d::SearchRes find_sd_min(util::Function && func) { return m_sd_searcher->find_min(std::move(func)); }
    /*
     * Returns function for which one dimensional minimization problem was solved.
     */
    const util::Function & last_sd_func() const noexcept { return m_sd_searcher->last_func(); }

private:
    min1d::MinSearcher * m_sd_searcher;     // Current one dimensional minimization method
};

} // namespace min_nd
