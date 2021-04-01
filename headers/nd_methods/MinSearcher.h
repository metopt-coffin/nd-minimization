#pragma once

#include "util/NFunction.h"
#include "util/ReplayData.h"

#include <vector>

namespace min_nd {

struct MinSearcher
{
    virtual std::vector<double> find_min(util::NFunction func) = 0;

    const util::NFunction & last_func() const { return *m_last_func; }

protected:
    util::ReplayData m_replay_data;
    std::optional<util::NFunction> m_last_func;
};

} // namespace min_nd
