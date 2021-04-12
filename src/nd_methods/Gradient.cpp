#include "nd_methods/Gradient.h"

#include "nd_methods/MinSearcher.h"

#include "util/ReplayData.h"
#include "util/Vector.h"
#include "util/VersionedData.h"

#include <iostream>

namespace min_nd {

SearchRes Gradient::find_min_impl()
{
    double eps_pow2 = m_eps * m_eps;
    double alpha = m_alpha;
    auto & func = curr_func();

    util::Vector curr_vec(func.dims());
    double f_curr = func(curr_vec);

    util::Vector next_vec(func.dims());
    double f_next;

    auto grad = func.grad(curr_vec);

    auto count_next = [&] {
        next_vec = curr_vec - alpha * grad;
        f_next = func(next_vec);
    };

    for (auto length = grad.length_pow2(); length >= eps_pow2; length = grad.length_pow2()) {
        for (count_next(); f_next >= f_curr && alpha > m_eps; count_next()) {
            alpha /= 2;
        }
        alpha = m_alpha;
        curr_vec = std::move(next_vec);
        f_curr = f_next;
        grad = func.grad(curr_vec);
    }

    return {curr_vec, f_curr};
}

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
    for (auto length = grad.length_pow2(); length >= eps_pow2; ++iter_num, length = grad.length_pow2()) {
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
    }

    return {curr_vec, f_curr, m_replay_data};
}

} // namespace min_nd
