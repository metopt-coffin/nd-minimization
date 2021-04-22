#include "nd_methods/ConjugateGrad.h"

#include "nd_methods/MinSearcher.h"

#include "util/Vector.h"
#include "util/VersionedData.h"

#include <iostream>

namespace min_nd {
/*
 * Idea: calculate the steepest direction (antigradient); 
 * compute coefficients alpha and beta, using give formulas;
 * update the position;
 * update the conjugate direction; 
 * repeat until exit condition is satisfied.
 *
 * Note: with a pure quadratic function the minimum is reached within N iterations.
 */
SearchRes ConjucateGrad::find_min_impl()
{
    const double eps_pow2 = m_eps * m_eps;
    auto & func = curr_func();

    /*
     * Initialize starting values.
     */
    util::Vector curr(func.dims());
    auto grad = func.grad(curr);
    auto p = grad * -1.;

    double grad_len_pow2 = grad.length_pow2();
    util::Vector a_by_p(func.dims());
    double beta = 0.;
    uint iter_num = 0; // to track number of iterations and to prevent infinite or very long cycles.

    while (grad_len_pow2 >= eps_pow2 && iter_num < MAX_ITER) {
        /*
         * "Restart" the method to avoid 
         * accumulating computing error
         */
        if (iter_num % func.dims() == 0) {
            beta = 0.;
        }

        a_by_p = func.a() * p; // compute A * p_k
        double alpha = grad_len_pow2 / (a_by_p * p); // compute coefficient alpha

        curr = curr + alpha * p; // update position
        grad = grad + alpha * a_by_p; // update gradient
        beta = grad.length_pow2() / grad_len_pow2; // compute coefficient beta
        p = beta * p - grad; //update the conjugate vector

        grad_len_pow2 = grad.length_pow2();
        iter_num++;
    }

    return {curr, func(curr)};
}

/*
 * Version with tracing output.
 */
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

    while (grad_len_pow2 >= eps_pow2 && iter_num < MAX_ITER) {
        if (iter_num % func.dims() == 0) {
            beta = 0.;
        }

        m_replay_data.emplace_back<util::VdComment>(iter_num, "x, f, grad, p");
        m_replay_data.emplace_back<util::VdVector>(iter_num, curr);
        m_replay_data.emplace_back<util::VdDouble>(iter_num, func(curr));
        m_replay_data.emplace_back<util::VdVector>(iter_num, grad);
        m_replay_data.emplace_back<util::VdVector>(iter_num, p);

        a_by_p = func.a() * p;
        double alpha = grad_len_pow2 / (a_by_p * p);
        m_replay_data.emplace_back<util::VdComment>(iter_num, "alpha, beta");
        m_replay_data.emplace_back<util::VdDouble>(iter_num, alpha);
        m_replay_data.emplace_back<util::VdDouble>(iter_num, beta);
        m_replay_data.emplace_back<util::VdComment>(iter_num, "x shift");
        m_replay_data.emplace_back<util::VdVector>(iter_num, alpha * p);

        curr = curr + alpha * p;
        grad = grad + alpha * a_by_p;
        beta = grad.length_pow2() / grad_len_pow2;
        p = beta * p - grad;

        grad_len_pow2 = grad.length_pow2();
        iter_num++;
    }

    m_replay_data.emplace_back<util::VdComment>(iter_num, "x, grad, p");
    m_replay_data.emplace_back<util::VdVector>(iter_num, curr);
    m_replay_data.emplace_back<util::VdVector>(iter_num, grad);
    m_replay_data.emplace_back<util::VdVector>(iter_num, p);

    return {curr, func(curr), m_replay_data};
}

} // namespace min_nd
