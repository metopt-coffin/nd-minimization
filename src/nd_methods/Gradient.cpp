#include "nd_methods/Gradient.h"

#include "nd_methods/MinSearcher.h"

#include "util/ReplayData.h"
#include "util/Vector.h"
#include "util/VersionedData.h"

#include <iostream>

namespace min_nd {
    /*
     * Idea: 
     * calculate the gradient;
     * find the next point in the steepest direction (in the direction of antigradient);
     * if the new value is less than current, then move to it and repeat;
     * otherwise, reduce the step size and try moving again.
     * Exit, when absolute value of the gradient is less than required precision.
     */
SearchRes Gradient::find_min_impl()
{
    /*
     * Initialize starting values;
     */
    double eps_pow2 = m_eps * m_eps;
    double alpha = m_alpha;
    auto & func = curr_func();

    util::Vector curr_vec(func.dims());
    double f_curr = func(curr_vec);

    util::Vector next_vec(func.dims());
    double f_next;

    auto grad = func.grad(curr_vec);

    // recalc function
    auto count_next = [&] {
        next_vec = curr_vec - alpha * grad;
        f_next = func(next_vec);
    };

    uint iter_num = 0;  // To prevent infinite or very long cycles
    for (auto length = grad.length_pow2(); length >= eps_pow2 && iter_num < MAX_ITER; length = grad.length_pow2()) {
        for (count_next(); f_next >= f_curr && alpha > m_eps; count_next()) {
            /*
             * New value is bigger than current. Reduce the step size and try again;
             */
            alpha /= 2;
        }
        /*
         * New value is less than current. Move to it and continue iterating.
         */
        alpha = m_alpha;
        curr_vec = std::move(next_vec);
        f_curr = f_next;
        grad = func.grad(curr_vec);
        iter_num++;
    }

    return {curr_vec, f_curr};
}

/*
 * Version with tracing output.
 */
TracedSearchRes Gradient::find_min_traced_impl()
{
    double eps_pow2 = m_eps * m_eps;
    double alpha = m_alpha;
    auto & func = curr_func();

    m_replay_data.emplace_back<util::VdComment>(0, "func dims");
    m_replay_data.emplace_back<util::VdDouble>(0, func.dims());

    util::Vector curr_vec(func.dims());
    double f_curr = func(curr_vec);

    util::Vector next_vec(func.dims());
    double f_next;

    auto grad = func.grad(curr_vec);

    auto count_next = [&] {
        next_vec = curr_vec - alpha * grad;
        f_next = func(next_vec);
    };

    uint iter_num = 0;
    for (auto length = grad.length_pow2(); length >= eps_pow2 && iter_num < MAX_ITER; length = grad.length_pow2()) {
        m_replay_data.emplace_back<util::VdComment>(iter_num, "x and f(x)");
        m_replay_data.emplace_back<util::VdVector>(iter_num, curr_vec);
        m_replay_data.emplace_back<util::VdDouble>(iter_num, f_curr);
        m_replay_data.emplace_back<util::VdComment>(iter_num, "grad");
        m_replay_data.emplace_back<util::VdVector>(iter_num, grad);

        for (count_next(); f_next >= f_curr && alpha > m_eps; count_next()) {
            alpha /= 2;
        }
        m_replay_data.emplace_back<util::VdComment>(iter_num, "shift");
        m_replay_data.emplace_back<util::VdVector>(iter_num, -alpha * grad);

        alpha = m_alpha;
        curr_vec = std::move(next_vec);
        f_curr = f_next;
        grad = func.grad(curr_vec);
        ++iter_num;
    }

    return {curr_vec, f_curr, m_replay_data};
}

} // namespace min_nd
