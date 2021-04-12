#pragma once

#include "util/NFunction.h"
#include "util/ReplayData.h"
#include "util/Vector.h"

#include <vector>

namespace min_nd {

struct SearchRes
{
    util::Vector min_point;
    double min;
};
struct TracedSearchRes : SearchRes
{
    const util::ReplayData & replay_data;
};

struct MinSearcher
{
    SearchRes find_min() { return find_min_impl(); }
    SearchRes find_min(util::NFunction func)
    {
        m_last_func.emplace(std::move(func));
        return find_min_impl();
    }

    TracedSearchRes find_min_traced() { return find_min_traced_impl(); }
    TracedSearchRes find_min_traced(util::NFunction func)
    {
        m_last_func.emplace(std::move(func));
        m_replay_data.clear();
        return find_min_traced_impl();
    }

    void set_func(util::NFunction func) { m_last_func.emplace(std::move(func)); }
    const util::NFunction & curr_func() const { return *m_last_func; }

protected:
protected:
    virtual SearchRes find_min_impl() = 0;
    virtual TracedSearchRes find_min_traced_impl() = 0;

protected:
    util::ReplayData m_replay_data;
    std::optional<util::NFunction> m_last_func;
};

} // namespace min_nd
