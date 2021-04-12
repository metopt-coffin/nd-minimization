#include "util/Function.h"

#include <iostream>

namespace util {

Function::Function(std::string as_string, CalculateFunc calculate, Bounds bounds)
    : m_as_string(std::move(as_string))
    , m_calculate(std::move(calculate))
    , m_bounds(bounds)
{}

Function::Function(CalculateFunc calculate, Bounds bounds)
    : Function("none", std::move(calculate), bounds)
{}

void Function::reset(std::string as_string, CalculateFunc calculate)
{
    m_as_string = std::move(as_string);
    m_calculate = std::move(calculate);
}


std::ostream & operator<<(std::ostream & out, const Function & func)
{
    return out << func.to_string() << " [" << func.m_bounds.from << ", " << func.m_bounds.to << ']';
}

} // namespace util
