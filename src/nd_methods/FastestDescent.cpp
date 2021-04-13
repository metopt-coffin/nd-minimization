#include "nd_methods/FastestDescent.h"

#include "nd_methods/MinSearcher.h"
#include "sd_methods/MinSearcher.h"

#include "util/Function.h"
#include "util/Vector.h"
#include "util/VersionedData.h"

namespace min_nd {
/*
 * Idea: after finding gradient of the function do not make a small step in the direction of the antigradient.
 * Instead, move, until the function decreases. 
 * After finding minimum on the chosen direction, find function's gradient again. 
 * Repeat the algorithm.
 */
SearchRes FastestDescent::find_min_impl()
{
    const double eps_pow2 = m_eps * m_eps;
    auto & func = curr_func();

    util::Vector curr(func.dims()); // Vector of current coordinates
    double f_curr = func(curr);

    util::Vector grad = func.grad(curr);
    min1d::SearchRes sd_min;    // Minimum found on the chosen direction
    while (grad.length_pow2() >= eps_pow2) {
        sd_min = find_sd_min({[&](double x) { return func(curr - x * grad); }, {0., m_alpha}});
        curr = curr - sd_min.min_point * grad;
        grad = func.grad(curr);
    }

    return {curr, sd_min.min};
}

/*
 * Version with tracing output.
 */
TracedSearchRes FastestDescent::find_min_traced_impl()
{
    const double eps_pow2 = m_eps * m_eps;
    auto & func = curr_func();

    util::Vector curr(func.dims());
    double f_curr = func(curr);

    util::Vector grad = func.grad(curr);
    min1d::SearchRes sd_min;

    uint iter_num = 0;
    while (grad.length_pow2() >= eps_pow2) {
        m_replay_data.emplace_back<util::VdComment>(iter_num, "x, grad");
        m_replay_data.emplace_back<util::VdVector>(iter_num, curr);
        m_replay_data.emplace_back<util::VdVector>(iter_num, grad);

        sd_min = find_sd_min({[&](double x) { return func(curr - x * grad); }, {0., m_alpha}});

        m_replay_data.emplace_back<util::VdComment>(iter_num, "found min, iterations needed");
        m_replay_data.emplace_back<util::VdPoint>(iter_num, sd_min.min_point, sd_min.min);
        m_replay_data.emplace_back<util::VdDouble>(iter_num, static_cast<double>(last_sd_func().call_count()));

        curr = curr - sd_min.min_point * grad;
        grad = func.grad(curr);

        ++iter_num;
    }

    return {curr, sd_min.min, m_replay_data};
}

} // namespace min_nd
