#pragma once

#include <type_traits> // enable_if, ...
#include <utility>     // std::forward

namespace wrt {

namespace fmt {

struct None_t {
  explicit None_t() = default;
};

constexpr auto None = None_t{};

} // namespace fmt

template <class _T, class _Format> struct FValue {
  using ValueType = _T;
  using FormatType = _Format;

  ValueType value;
  FormatType fmt;
};

namespace details {

template <class T> struct IsFValue : std::false_type {};

template <class T, class Fmt>
struct IsFValue<FValue<T, Fmt>> : std::true_type {};

template <class T> constexpr bool IsFValue_v = IsFValue<T>::value;

template <class T> struct IsNoneFValue : std::false_type {};

template <class T, class Fmt>
struct IsNoneFValue<FValue<T, Fmt>>
    : std::is_same<std::decay_t<Fmt>, fmt::None_t> {};

template <class T> constexpr bool IsNoneFValue_v = IsNoneFValue<T>::value;

} // namespace details

template <class Fmt, class T>
constexpr auto AsFValue(T &&val, Fmt &&fmt)
    -> FValue<std::decay_t<T>, std::decay_t<Fmt>> {
  return FValue<std::decay_t<T>, std::decay_t<Fmt>>{
      std::forward<T>(val),
      std::forward<Fmt>(fmt),
  };
}

template <class T> constexpr auto AsFValue(T &&val) {
  return AsRange(std::forward<T>(val), fmt::None);
}

template <class Fmt, class T>
constexpr auto TieAsFValue(T &val, Fmt &&fmt)
    -> FValue<T &, std::decay_t<Fmt> &> {
  return FValue<T &, std::decay_t<Fmt>>{
      val,
      std::forward<Fmt>(fmt),
  };
}

template <class T> constexpr auto TieAsFValue(T &val) {
  return TieAsRange(std::forward<T>(val), fmt::None);
}

template <class Fmt, class T>
constexpr auto ForwardAsFValue(T &&val, Fmt &&fmt)
    -> FValue<T &&, std::decay_t<Fmt>> {
  return FValue<T &&, std::decay_t<Fmt>>{
      std::forward<T>(val),
      std::forward<Fmt>(fmt),
  };
}

template <class T> constexpr auto ForwardAsFValue(T &&val) {
  return ForwardAsFValue(std::forward<T>(val), fmt::None);
}

template <
    class Sink, class T, class...,
    std::enable_if_t<not details::IsFValue_v<std::decay_t<T>>, bool> = true>
constexpr auto WriteTo(Sink &&sink, T &&val) -> void {
  WriteTo(std::forward<Sink>(sink),
          ForwardAsFValue(std::forward<T>(val), fmt::None));
}

template <class Sink, class T, class U, class... Others>
constexpr auto WriteTo(Sink &&sink, T &&t, U &&u, Others &&...others) -> void {
  WriteTo(std::forward<Sink>(sink), std::forward<T>(t));
  WriteTo(std::forward<Sink>(sink), std::forward<U>(u),
          std::forward<Others>(others)...);
};

} // namespace wrt
