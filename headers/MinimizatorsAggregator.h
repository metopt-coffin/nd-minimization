#pragma once

#include "nd_methods/MinSearcher.h"
#include "sd_methods/MinSearcher.h"

#include "util/DiagMatrix.h"
#include "util/Vector.h"

#include <memory>
#include <optional>
#include <string>
#include <variant>

namespace min_nd {

struct MinimizatorsAggregator
{
    using MaybeErrorText = std::optional<std::string>;

    using NMethodPtr = std::unique_ptr<min_nd::MinSearcher>;
    using SDMethodPtr = std::unique_ptr<min1d::MinSearcher>;
    using NFuncRef = std::reference_wrapper<util::NFunction>;

    MaybeErrorText setup();

    MaybeErrorText select_nd_method(uint method_id);
    MaybeErrorText select_sd_method(uint method_id) { return select(method_id, m_sd_methods.size(), m_curr_sd_method); }
    MaybeErrorText select_function(uint func_id);

    MaybeErrorText add_function(util::DiagMatrix a, util::Vector b, double c, double eigenvalue);

    SearchRes search_min() { return curr_nd_searcher().find_min(); }
    TracedSearchRes search_min_traced() { return curr_nd_searcher().find_min_traced(); }

private:
    util::NFunction & curr_func() noexcept { return m_funcs[m_curr_func]; }
    min_nd::MinSearcher & curr_nd_searcher() noexcept { return *m_nd_methods[m_curr_nd_method]; }
    min1d::MinSearcher & curr_sd_searcher() noexcept { return *m_sd_methods[m_curr_sd_method]; }

    static MaybeErrorText select(uint method_id, std::size_t vec_size, std::size_t & to_select_idx);

private:
    std::vector<util::NFunction> m_funcs;
    std::vector<NMethodPtr> m_nd_methods;
    std::vector<SDMethodPtr> m_sd_methods;

    std::size_t m_curr_func = 0;
    std::size_t m_curr_nd_method = 0;
    std::size_t m_curr_sd_method = 0;
};


} // namespace min_nd
