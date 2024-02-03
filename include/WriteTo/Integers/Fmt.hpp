#pragma once

#include "../Core.hpp"

#include <cstdint>
#include <type_traits>
#include <utility>

namespace wrt {
namespace fmt {
struct AsHex_t {
  explicit AsHex_t() = default;
};

template <class T> constexpr auto AsHex(T &&value) {
  return ForwardAsFValue(std::forward<T>(value), AsHex_t{});
}

struct AsOct_t {
  explicit AsOct_t() = default;
};

template <class T> constexpr auto AsOct(T &&value) {
  return ForwardAsFValue(std::forward<T>(value), AsOct_t{});
}

struct AsDec_t {
  explicit AsDec_t() = default;
};

template <class T> constexpr auto AsDec(T &&value) {
  return ForwardAsFValue(std::forward<T>(value), AsDec_t{});
}

} // namespace fmt

namespace details {

template <class T, class Fmt>
struct IsValidIntFormat
    : std::conjunction<std::is_integral<T>,
                       std::disjunction<std::is_same<Fmt, fmt::AsHex_t>,
                                        std::is_same<Fmt, fmt::AsOct_t>,
                                        std::is_same<Fmt, fmt::AsDec_t>>> {};

template <class T, class Fmt, class Any>
struct IsValidIntFormat<FValue<T, Fmt>, Any>
    : IsValidIntFormat<std::decay_t<T>, std::decay_t<Fmt>> {};

template <class T, class Fmt = void>
constexpr bool IsValidIntFormat_v = IsValidIntFormat<T, Fmt>::value;

template <class T, class Fmt> constexpr auto GetMaxDigitWidthFor() -> int {
  static_assert(IsValidIntFormat_v<T, Fmt>);

  constexpr auto IntSize = sizeof(T);

  if constexpr (std::is_same_v<Fmt, fmt::AsHex_t>) {
    return (IntSize * 2);
  } else if constexpr (std::is_same_v<Fmt, fmt::AsDec_t>) {
    if constexpr (IntSize == sizeof(std::uint8_t)) {
      return 3 + 1;
    } else if constexpr (IntSize == sizeof(std::uint16_t)) {
      // 5 for unsigned, +1 because of signed value
      return 5 + 1;
    } else if constexpr (IntSize == sizeof(std::uint32_t)) {
      // 10 for unsigned, +1 because of signed value
      return 10 + 1;
    } else if constexpr (IntSize == sizeof(std::uint64_t)) {
      // Signed is 19
      return 20;
    } else {
      static_assert(IntSize == 0, "Unknown Integer size.");
    }
  } else if constexpr (std::is_same_v<Fmt, fmt::AsOct_t>) {
    if constexpr (IntSize == sizeof(std::uint8_t)) {
      return 3;
    } else if constexpr (IntSize == sizeof(std::uint16_t)) {
      return 6;
    } else if constexpr (IntSize == sizeof(std::uint32_t)) {
      return 11;
    } else if constexpr (IntSize == sizeof(std::uint64_t)) {
      return 22;
    } else {
      static_assert(IntSize == 0, "Unknown Integer size.");
    }
  } else {
    static_assert(
        sizeof(Fmt) == 0,
        "Unknown Integer format. Only AsDec, AsHex or AsOct are allowed.");
  }
}

template <class T, class..., class FValueType = std::decay_t<T>,
          std::enable_if_t<details::IsFValue_v<FValueType>, bool> = true>
constexpr auto GetMaxDigitWidthFor(T &&) -> int {
  return GetMaxDigitWidthFor<std::decay_t<typename FValueType::ValueType>,
                             std::decay_t<typename FValueType::FormatType>>();
}

template <class T, class Fmt> constexpr auto GetIntFmtOf() -> const char * {
  static_assert(IsValidIntFormat_v<T, Fmt>);

  constexpr auto IntSize = sizeof(T);

  if constexpr (std::is_same_v<Fmt, fmt::AsDec_t>) {
    if constexpr (IntSize == sizeof(std::uint8_t)) {
      if constexpr (std::is_signed_v<T>) {
        return "%hhd";
      } else {
        return "%hhu";
      }
    } else if constexpr (IntSize == sizeof(std::uint16_t)) {
      if constexpr (std::is_signed_v<T>) {
        return "%hd";
      } else {
        return "%hu";
      }
    } else if constexpr (IntSize == sizeof(std::uint32_t)) {
      if constexpr (std::is_signed_v<T>) {
        return "%d";
      } else {
        return "%u";
      }
    } else if constexpr (IntSize == sizeof(std::uint64_t)) {
      if constexpr (std::is_signed_v<T>) {
        return "%ld";
      } else {
        return "%lu";
      }
    } else {
      static_assert(IntSize == 0, "Unknown Integer size.");
    }
  } else if constexpr (std::is_same_v<Fmt, fmt::AsHex_t>) {
    return "%0*X";
  } else if constexpr (std::is_same_v<Fmt, fmt::AsOct_t>) {
    return "%0*o";
  } else {
    static_assert(
        sizeof(Fmt) == 0,
        "Unknown Integer format. Only AsDec, AsHex or AsOct are allowed.");
  }
}

template <class T, class..., class FValueType = std::decay_t<T>,
          std::enable_if_t<details::IsFValue_v<FValueType>, bool> = true>
constexpr auto GetIntFmtOf(T &&) -> const char * {
  return GetIntFmtOf<std::decay_t<typename FValueType::ValueType>,
                     std::decay_t<typename FValueType::FormatType>>();
}

} // namespace details

template <
    class Sink, class T, class NoneFmt, class...,
    std::enable_if_t<std::is_integral_v<std::decay_t<T>> and
                         std::is_same_v<std::decay_t<NoneFmt>, fmt::None_t>,
                     bool> = true>
constexpr auto WriteTo(Sink &&sink, FValue<T &&, NoneFmt> int_val) -> void {
  // Default to Decimal when no format is provided
  WriteTo(std::forward<Sink>(sink), fmt::AsDec(std::forward<T>(int_val.value)));
}

} // namespace wrt
