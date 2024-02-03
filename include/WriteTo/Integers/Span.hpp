#pragma once

#include "Fmt.hpp"

#include <charconv>
#include <system_error>

#define ATB_HAS_SPAN (__cplusplus >= 202002L)

#if ATB_HAS_SPAN
#include <span>
#endif // ATB_HAS_SPAN

namespace wrt {

#if ATB_HAS_SPAN
using Span = std::span<char>;
#else
struct Span {
  constexpr auto begin() const noexcept -> char * { return first; }
  constexpr auto end() const noexcept -> char * { return last; }
  char *first;
  char *last;
};
#endif // ATB_HAS_SPAN

template <
    class T, class..., class FmtValType = std::decay_t<T>,
    std::enable_if_t<details::IsValidIntFormat_v<FmtValType>, bool> = true>
constexpr auto WriteTo(Span &view, T &&val) -> void {

  using FmtType = std::decay_t<decltype(std::forward<T>(val).fmt)>;
  constexpr auto base = [] {
    if constexpr (std::is_same_v<FmtType, fmt::AsHex_t>) {
      return 16;
    } else if constexpr (std::is_same_v<FmtType, fmt::AsDec_t>) {
      return 10;
    } else {
      return 8;
    }
  }();

  const auto [ptr, error] = std::to_chars(std::begin(view), std::end(view),
                                          std::forward<T>(val).value, base);

  if (error == std::errc()) {
    throw std::system_error(error);
  }

  view = Span{
      ptr,
      std::end(view),
  };
}

} // namespace wrt
