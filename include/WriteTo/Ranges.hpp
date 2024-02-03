#pragma once

#include "Core.hpp"

#include <iterator>
#include <type_traits>

namespace wrt {

namespace details {

template <class T, class = void> struct IsARange : std::false_type {};

template <class T>
struct IsARange<T, std::void_t<decltype(std::begin(std::declval<T>())),
                               decltype(std::end(std::declval<T>()))>>
    : std::true_type {};

template <class T> constexpr bool IsARange_v = IsARange<T>::value;

} // namespace details

namespace fmt {

template <class Sep, class Fmt> struct AsRange_t {
  Sep separator;
  Fmt element_fmt;
};

template <class Range, class Sep, class Fmt>
constexpr auto AsRange(Range &&range, Sep &&sep, Fmt &&fmt) {
  return ForwardAsFValue(std::forward<Range>(range), AsRange_t<Sep &&, Fmt &&>{
                                                         std::forward<Sep>(sep),
                                                         std::forward<Fmt>(fmt),
                                                     });
}

template <class Range, class Sep>
constexpr auto AsRange(Range &&range, Sep &&sep) {
  return AsRange(std::forward<Range>(range), std::forward<Sep>(sep),
                 wrt::fmt::None);
}

template <
    class Sep, class Range, class Fmt, class...,
    std::enable_if_t<details::IsARange_v<std::decay_t<Range>>, bool> = true>
constexpr auto Join(Sep &&sep, Range &&range, Fmt &&fmt) {
  return AsRange(std::forward<Range>(range), std::forward<Sep>(sep),
                 std::forward<Fmt>(fmt));
}

template <
    class Sep, class Range, class...,
    std::enable_if_t<details::IsARange_v<std::decay_t<Range>>, bool> = true>
constexpr auto Join(Sep &&sep, Range &&range) {
  return AsRange(std::forward<Range>(range), std::forward<Sep>(sep));
}

} // namespace fmt

namespace details {

template <class T> struct IsAsRangeFmt : std::false_type {};

template <class T, class U>
struct IsAsRangeFmt<fmt::AsRange_t<T, U>> : std::true_type {};

template <class T> constexpr bool IsAsRangeFmt_v = IsAsRangeFmt<T>::value;

template <class, class Fmt> struct IsValidRangeFValue : IsAsRangeFmt<Fmt> {};

template <class T, class Fmt, class Ignored>
struct IsValidRangeFValue<FValue<T, Fmt>, Ignored>
    : IsValidRangeFValue<std::decay_t<T>, std::decay_t<Fmt>> {};

template <class T, class Fmt = void>
constexpr bool IsValidRangeFValue_v = IsValidRangeFValue<T, Fmt>::value;

} // namespace details

template <
    class Sink, class T, class..., class FValueType = std::decay_t<T>,
    std::enable_if_t<details::IsValidRangeFValue_v<FValueType>, bool> = true>
constexpr auto WriteTo(Sink &&sink, T &&frange) -> void {

  auto &&range = std::forward<T>(frange).value;

  auto first = std::cbegin(std::forward<decltype(range)>(range));
  const auto last = std::cend(std::forward<decltype(range)>(range));

  if (first != last) {

    auto &&fmt = std::forward<T>(frange).fmt;
    auto &&[sep, element_fmt] = std::forward<decltype(fmt)>(fmt);

    WriteTo(std::forward<Sink>(sink),
            ForwardAsFValue(*first,
                            std::forward<decltype(element_fmt)>(element_fmt)));
    ++first;

    while (first != last) {
      WriteTo(std::forward<Sink>(sink), std::forward<decltype(sep)>(sep),
              ForwardAsFValue(
                  *first, std::forward<decltype(element_fmt)>(element_fmt)));
      ++first;
    }
  }
}

} // namespace wrt
