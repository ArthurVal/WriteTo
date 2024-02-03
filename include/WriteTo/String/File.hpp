#pragma once

#include "Fmt.hpp"

#include <cstdio>
#include <type_traits>

namespace wrt {

template <
    class T, class..., class FmtValType = std::decay_t<T>,
    std::enable_if_t<details::IsValidStringFormat_v<FmtValType>, bool> = true>
static inline auto WriteTo(std::FILE *stream, T &&t) -> void {
  using ValueType = std::decay_t<typename FmtValType::ValueType>;

  if constexpr (std::is_same_v<ValueType, char>) {
    std::fprintf(stream, "%c", t.value);
  } else {
    auto &&str = [&]() -> decltype(auto) {
      if constexpr (std::is_same_v<ValueType, std::string_view>) {
        return std::forward<decltype(t.value)>(t.value);
      } else {
        return std::string_view{std::forward<decltype(t.value)>(t.value)};
      }
    }();

    std::fprintf(stream, "%.*s", (int)str.size(), str.data());
  }
}

} // namespace wrt
