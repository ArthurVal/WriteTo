#pragma once

#include "Fmt.hpp"

#include <array>
#include <iomanip>
#include <ostream>

namespace wrt {

template <
    class T, class..., class FmtValType = std::decay_t<T>,
    std::enable_if_t<details::IsValidIntFormat_v<FmtValType>, bool> = true>
constexpr auto WriteTo(std::ostream &os, T &&val) -> void {

  const auto os_flags = os.flags();
  using FmtType = std::decay_t<typename FmtValType::FormatType>;

  if constexpr (std::is_same_v<FmtType, fmt::AsDec_t>) {
    os << std::dec;
  } else {
    os << std::setfill('0');
    os << std::right;
    os << std::setw(details::GetMaxDigitWidthFor(val));
    if constexpr (std::is_same_v<FmtType, fmt::AsHex_t>) {
      os << std::hex;
    } else if constexpr (std::is_same_v<FmtType, fmt::AsOct_t>) {
      os << std::oct;
    } else {
      static_assert(
          sizeof(FmtType) == 0,
          "Unknown Integer format. Only AsDec, AsHex or AsOct are allowed.");
    }
  }

  if constexpr (sizeof(val.value) == 1) {
    os << +val.value;
  } else {
    os << val.value;
  }

  os.flags(os_flags);
}

} // namespace wrt
