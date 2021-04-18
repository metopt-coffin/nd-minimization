#include "util/VersionedData.h"

#include "util/Vector.h"

namespace util {

VdVector::VdVector(uint version, const Vector & vec)
    : VersionedData(version)
    , m_vec(new Vector(vec))
{}

} // namespace util
