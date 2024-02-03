#pragma once

#include "Fmt.hpp"

#include <cstdio>

namespace wrt {

template <
    class T, class..., class FmtValType = std::decay_t<T>,
    std::enable_if_t<details::IsValidIntFormat_v<FmtValType>, bool> = true>
constexpr auto WriteTo(std::FILE *stream, T &&val) -> void {

  using FmtType = std::decay_t<typename FmtValType::FormatType>;

  if constexpr (std::is_same_v<FmtType, fmt::AsDec_t>) {
    std::fprintf(stream, details::GetIntFmtOf(val), val.value);
  } else {
    std::fprintf(stream, details::GetIntFmtOf(val),
                 details::GetMaxDigitWidthFor(val), val.value);
  }
}

} // namespace wrt
