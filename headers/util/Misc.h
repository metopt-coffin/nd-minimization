#pragma once

#include <functional>
#include <memory>
#include <type_traits>

namespace util {

using CalculateFunc = std::function<double(double)>;
using CalculateNFunc = std::function<double(const std::vector<double> &)>;

template <class... Funcs>
struct Overloaded : public Funcs...
{
    using Funcs::operator()...;

    template <class... Fs>
    constexpr Overloaded(Fs &&... fs)
        : Funcs(std::forward<Fs>(fs))...
    {}
};

template <class... Funcs>
constexpr auto overload(Funcs &&... funcs) { return Overloaded<std::decay_t<Funcs>...>(std::forward<Funcs>(funcs)...); }

template <class... Types>
struct TypeList;

namespace detail {

template <template <class... DestTypes> class Getter, class List>
struct FromTypeListImpl;

template <template <class... DestTypes> class Getter, class... SrcTypes>
struct FromTypeListImpl<Getter, TypeList<SrcTypes...>>
{
    using type = Getter<SrcTypes...>;
};
} // namespace detail

template <template <class... DestTypes> class Getter, class List>
using FromTypeList = typename detail::FromTypeListImpl<Getter, List>::type;
} // namespace util
