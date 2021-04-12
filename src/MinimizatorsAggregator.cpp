#include "MinimizatorsAggregator.h"

#include "nd_methods/FastestDescent.h"
#include "nd_methods/Gradient.h"
#include "sd_methods/Golden.h"

#include "util/Vector.h"

#include <optional>

namespace min_nd {

/*static*/ auto MinimizatorsAggregator::select(uint method_id, std::size_t vec_size, std::size_t & to_select) -> MaybeErrorText
{
    if (method_id < vec_size) {
        to_select = method_id;
        return std::nullopt;
    } else {
        return {"Non available index"};
    }
}

auto MinimizatorsAggregator::setup() -> MaybeErrorText
{
    m_sd_methods.emplace_back(new min1d::Golden(0.000001));

    m_nd_methods.emplace_back(new Gradient(0.000001, 1000.));
    m_nd_methods.emplace_back(new FastestDescent(0.000001, 1000., curr_sd_searcher()));
    return std::nullopt;
}

auto MinimizatorsAggregator::select_nd_method(uint method_id) -> MaybeErrorText
{
    auto err = select(method_id, m_nd_methods.size(), m_curr_nd_method);
    if (!err && m_curr_func < m_funcs.size()) {
        curr_nd_searcher().set_func(curr_func());
    }
    return err;
}

auto MinimizatorsAggregator::select_function(uint func_id) -> MaybeErrorText
{
    auto err = select(func_id, m_funcs.size(), m_curr_func);
    if (!err && m_curr_nd_method < m_nd_methods.size()) {
        curr_nd_searcher().set_func(curr_func());
    }
    return err;
}

auto MinimizatorsAggregator::add_function(util::QuadMatrix a, util::Vector b, double c) -> MaybeErrorText
{
    m_funcs.emplace_back(std::move(a), std::move(b), c);
    return std::nullopt;
}

} // namespace min_nd
