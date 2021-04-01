#pragma once

#include "Function.h"
#include "Misc.h"
#include "VersionedData.h"

#include <cstdint>
#include <iterator>
#include <memory>
#include <type_traits>
#include <vector>

namespace util {

struct ReplayData
{
private:
    template <class Container>
    using ContValType = typename std::decay_t<Container>::value_type;


    template <class T>
    static constexpr bool IsVersionedData = std::is_base_of_v<VersionedData, T>;

public:
    using VdDataPtr = std::unique_ptr<const VersionedData>;
    using iterator = std::vector<VdDataPtr>::const_iterator;

    iterator begin() const { return m_data.begin(); }
    iterator end() const { return m_data.end(); }


    template <class VdData>
    auto push_back(VdData && vd_data) -> std::enable_if_t<std::is_base_of_v<VersionedData, std::decay_t<VdData>>>
    {
        using ValueType = std::decay_t<VdData>;

        m_data.emplace_back(std::make_unique<const ValueType>(std::forward<VdData>(vd_data)));
        m_total_versions = std::max(m_total_versions, vd_data.version());
    }


    template <class VdDataType, class... Args>
    void emplace_back(Args &&... args) { m_data.emplace_back(std::make_unique<VdDataType>(std::forward<Args>(args)...)); }

    void clear()
    {
        m_data.clear();
        m_total_versions = 0;
    }

private:
    std::vector<VdDataPtr> m_data;
    uint m_total_versions = 0;
};

} // namespace util
