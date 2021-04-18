#pragma once

#include "util/Function.h"
#include "util/ReplayData.h"

#include <ostream>
#include <string_view>

namespace min1d {

struct SearchRes
{
    double min_point, min;
};

struct TracedSearchRes : SearchRes
{
    const util::ReplayData & replay_data;
};

struct MinSearcher
{
    virtual ~MinSearcher() = default;

    SearchRes find_min(util::Function func)
    {
        m_last_func.emplace(std::move(func));
        m_last_func->reset();
        return find_min_impl();
    }
    TracedSearchRes find_min_tracked(util::Function func)
    {
        m_last_func.emplace(std::move(func));
        m_replay_data.clear();
        m_last_func->reset();
        return find_min_tracked_impl();
    }

    const util::ReplayData & replay_data() const noexcept { return m_replay_data; }

    const util::Function & last_func() const noexcept { return *m_last_func; }

public:
    virtual std::string_view method_name() const noexcept = 0;

protected:
    virtual SearchRes find_min_impl() noexcept = 0;
    virtual TracedSearchRes find_min_tracked_impl() noexcept = 0;

protected:
    util::ReplayData m_replay_data;
    std::optional<util::Function> m_last_func;
};

} // namespace min1d
