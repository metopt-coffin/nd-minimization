#pragma once

#include "MinSearcher.h"
#include "nd_methods/Gradient.h"
#include "sd_methods/MinSearcher.h"

#include "util/Function.h"
#include "util/Vector.h"

namespace min_nd {

struct FastestDescent : Gradient
{
    FastestDescent(double eps, double max_step, min1d::MinSearcher & sd_searcher)
        : Gradient(eps, max_step)
        , m_sd_searcher(&sd_searcher)
    {}

public:
    void set_sd_searcher(min1d::MinSearcher & sd_searcher) { m_sd_searcher = &sd_searcher; }

protected:
    SearchRes find_min_impl() override;
    TracedSearchRes find_min_traced_impl() override;

protected:
    min1d::SearchRes find_sd_min(util::Function && func) { return m_sd_searcher->find_min(std::move(func)); }
    const util::Function & last_sd_func() const noexcept { return m_sd_searcher->last_func(); }

private:
    min1d::MinSearcher * m_sd_searcher;
};

} // namespace min_nd
