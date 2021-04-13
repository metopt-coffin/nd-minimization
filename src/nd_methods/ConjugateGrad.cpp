#include "nd_methods/ConjugateGrad.h"

#include "nd_methods/MinSearcher.h"

#include "util/Vector.h"
#include "util/VersionedData.h"

#include <iostream>

namespace min_nd {

SearchRes ConjucateGrad::find_min_impl()
{
    const double eps_pow2 = m_eps * m_eps;
    auto & func = curr_func();

    util::Vector curr(func.dims());
    auto grad = func.grad(curr);
    auto p = grad * -1.;

    double grad_len_pow2 = grad.length_pow2();
    util::Vector a_by_p(func.dims());
    double beta = 0.;
    uint iter_num = 0;

    while (grad_len_pow2 >= eps_pow2) {
        if (iter_num % func.dims() == 0) {
            beta = 0.;
        }

        a_by_p = func.a() * p;
        double alpha = grad_len_pow2 / (a_by_p * p);

        curr = curr + alpha * p;
        grad = grad + alpha * a_by_p;
        beta = grad.length_pow2() / grad_len_pow2;
        p = beta * p - grad;

        grad_len_pow2 = grad.length_pow2();
        iter_num++;
    }

    return {curr, func(curr)};
}

TracedSearchRes ConjucateGrad::find_min_traced_impl()
{
    const double eps_pow2 = m_eps * m_eps;
    auto & func = curr_func();

    util::Vector curr(func.dims());
    auto grad = func.grad(curr);
    auto p = grad * -1.;

    double grad_len_pow2 = grad.length_pow2();
    util::Vector a_by_p(func.dims());
    double beta = 0.;
    uint iter_num = 0;

    while (grad_len_pow2 >= eps_pow2) {
        if (iter_num % func.dims() == 0) {
            beta = 0.;
        }

        m_replay_data.emplace_back<util::VdComment>(iter_num, "x, grad, p");
        m_replay_data.emplace_back<util::VdVector>(iter_num, curr);
        m_replay_data.emplace_back<util::VdVector>(iter_num, grad);
        m_replay_data.emplace_back<util::VdVector>(iter_num, p);

        a_by_p = func.a() * p;
        double alpha = grad_len_pow2 / (a_by_p * p);

        m_replay_data.emplace_back<util::VdComment>(iter_num, "x shift");
        m_replay_data.emplace_back<util::VdVector>(iter_num, alpha * p);

        curr = curr + alpha * p;
        grad = grad + alpha * a_by_p;
        beta = grad.length_pow2() / grad_len_pow2;
        p = beta * p - grad;

        grad_len_pow2 = grad.length_pow2();
        iter_num++;
    }

    return {curr, func(curr), m_replay_data};
}

} // namespace min_nd
