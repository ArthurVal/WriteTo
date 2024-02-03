#pragma once

#include "../Core.hpp"

#include <string_view>
#include <type_traits>

namespace wrt {
namespace fmt {
// Currently no FMT provided
} // namespace fmt

namespace details {

template <class T, class Fmt>
struct IsValidStringFormat
    : std::disjunction<std::is_constructible<std::string_view, T>> {};

template <class T, class Fmt, class Any>
struct IsValidStringFormat<FValue<T, Fmt>, Any>
    : IsValidStringFormat<std::decay_t<T>, std::decay_t<Fmt>> {};

template <class T, class Fmt = void>
constexpr bool IsValidStringFormat_v = IsValidStringFormat<T, Fmt>::value;

} // namespace details

} // namespace wrt
