#pragma once

#include "Misc.h"

#include <cassert>
#include <string>

namespace util {

struct Vector;


#define M(vd_data_name) vd_data_name##Kind,
enum struct VdDataKind
{
#include "VersionedDataKinds.inl"
};

#define M(vd_data_name) struct vd_data_name;
#include "VersionedDataKinds.inl"

struct VersionedData
{
    explicit VersionedData(uint version)
        : m_version(version)
    {}
    virtual ~VersionedData() = default;

    virtual VdDataKind get_kind() const noexcept = 0;

    template <class Func>
    auto call_func(Func && func) const
    {
#define M(type) \
    case VdDataKind::type##Kind: return func(reinterpret_cast<const type &>(*this));

        switch (get_kind()) {
#include "VersionedDataKinds.inl"
        default: assert(false && "There is no such VersionedData kind");
        }
    }

    uint version() const noexcept { return m_version; }

protected:
    uint m_version;
};

struct VdPoint : VersionedData
{
    VdPoint(uint version, double x, double y)
        : VersionedData(version)
        , x(x)
        , y(y)
    {}

    VdDataKind get_kind() const noexcept override { return VdDataKind::VdPointKind; }

    double x, y;
};

struct VdParabole : VersionedData
{
    VdParabole(uint version, double a, double b, double c)
        : VersionedData(version)
        , a(a)
        , b(b)
        , c(c)
    {}

    VdDataKind get_kind() const noexcept override { return VdDataKind::VdParaboleKind; }

    double a, b, c;
};

struct VdSegment : VersionedData
{
    VdSegment(uint version, double l, double r)
        : VersionedData(version)
        , l(l)
        , r(r)
    {}

    VdDataKind get_kind() const noexcept override { return VdDataKind::VdSegmentKind; }

    double l, r;
};

struct VdComment : VersionedData
{
    VdComment(uint version, std::string comment)
        : VersionedData(version)
        , comment(std::move(comment))
    {}

    VdDataKind get_kind() const noexcept override { return VdDataKind::VdCommentKind; }

    std::string comment;
};

struct VdDouble : VersionedData
{
    VdDouble(uint version, double value)
        : VersionedData(version)
        , value(value)
    {}

    VdDataKind get_kind() const noexcept override { return VdDataKind::VdDoubleKind; }

    double value;
};

struct VdVector : VersionedData
{
    VdVector(uint version, const Vector & vec);

    const Vector & vec() const { return *m_vec; }

    VdDataKind get_kind() const noexcept override { return VdDataKind::VdVectorKind; }

private:
    std::unique_ptr<Vector> m_vec;
};

} // namespace util
