#pragma once

#include "Core.hpp"

#include <tuple>
#include <type_traits>
#include <utility>

namespace wrt {

namespace fmt {

template <class Sep> struct WithSeparator_t { Sep sep; };

template <class Sep, class... T> constexpr auto Join(Sep &&sep, T &&...values) {
  return AsFValue(std::forward_as_tuple(std::forward<T>(values)...),
                  WithSeparator_t<Sep &&>{std::forward<Sep>(sep)});
}

} // namespace fmt

namespace details {

template <class T> struct IsWithSeparator : std::false_type {};

template <class Sep>
struct IsWithSeparator<fmt::WithSeparator_t<Sep>> : std::true_type {};

template <class T> constexpr bool IsWithSeparator_v = IsWithSeparator<T>::value;

template <class T, class Fmt> struct IsValidTupleFValue : std::false_type {};

template <class AnyFmt, class... T>
struct IsValidTupleFValue<std::tuple<T...>, AnyFmt> : std::true_type {};

template <class T, class Fmt, class Ignored>
struct IsValidTupleFValue<FValue<T, Fmt>, Ignored>
    : IsValidTupleFValue<std::decay_t<T>, std::decay_t<Fmt>> {};

template <class T, class Fmt = void>
constexpr bool IsValidTupleFValue_v = IsValidTupleFValue<T, Fmt>::value;

} // namespace details

template <class Sink, class FValue, class...,
          std::enable_if_t<details::IsValidTupleFValue_v<std::decay_t<FValue>>,
                           bool> = true>
constexpr auto WriteTo(Sink &&sink, FValue &&ftuple) -> void {
  auto &&tuple = std::forward<FValue>(ftuple).value;
  auto &&fmt = std::forward<FValue>(ftuple).fmt;

  std::apply(
      [&](auto &&value, auto &&...others) {
        WriteTo(std::forward<Sink>(sink), std::forward<decltype(value)>(value));

        if constexpr (sizeof...(others) > 0) {
          if constexpr (details::IsWithSeparator_v<
                            std::decay_t<decltype(fmt)>>) {
            auto &&sep = std::forward<decltype(fmt)>(fmt).sep;

            (WriteTo(std::forward<Sink>(sink), std::forward<decltype(sep)>(sep),
                     std::forward<decltype(others)>(others)),
             ...);

          } else {
            WriteTo(std::forward<Sink>(sink),
                    std::forward<decltype(others)>(others)...);
          }
        }
      },
      std::forward<decltype(tuple)>(tuple));
}

} // namespace wrt
