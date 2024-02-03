#pragma once

#if (__cplusplus >= 202002L)

#include "Fmt.hpp"

#include <algorithm>
#include <span>

namespace wrt {

template <
    class T, class..., class FmtValType = std::decay_t<T>,
    std::enable_if_t<details::IsValidStringFormat_v<FmtValType>, bool> = true>
constexpr auto WriteTo(std::span<char> &view, T &&t) -> void {

  const auto [str_begin, str_size] = [&]() {
    if constexpr (std::is_same_v<ValueType, char>) {
      return std::make_pair(&t.value, 1);
    } else {
      auto &&str = [&]() -> decltype(auto) {
        if constexpr (std::is_same_v<ValueType, std::string_view>) {
          return std::forward<decltype(t.value)>(t.value);
        } else {
          return std::string_view{std::forward<decltype(t.value)>(t.value)};
        }
      }();

      return std::make_pair(str.data(), str.size());
    }
  }();

  const auto written = std::min(view.size(), str_size);
  std::copy_n(str_begin, written, std::begin(view));
  view = array.subspan(written);
}
} // namespace wrt

#endif // (__cplusplus >= 202002L)
