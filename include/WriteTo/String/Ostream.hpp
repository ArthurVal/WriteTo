#pragma once

#include "Fmt.hpp"

#include <ostream>

namespace wrt {

template <
    class T, class..., class FmtValType = std::decay_t<T>,
    std::enable_if_t<details::IsValidStringFormat_v<FmtValType>, bool> = true>
static inline auto WriteTo(std::ostream &os, T &&t) -> void {
  using ValueType = std::decay_t<typename FmtValType::ValueType>;

  if constexpr (std::is_same_v<ValueType, char> or
                std::is_same_v<ValueType, std::string_view>) {
    os << std::forward<decltype(t.value)>(t.value);
  } else {
    os << std::string_view{std::forward<decltype(t.value)>(t.value)};
  }
}

} // namespace wrt
