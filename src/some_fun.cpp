#include "WriteTo/Integers/All.hpp"
#include "WriteTo/String/All.hpp"

#include "WriteTo/Ranges.hpp"
#include "WriteTo/Tuples.hpp"

#include <array>
#include <cstdint>
#include <iostream>

int main() {

  auto toto = std::array{"Coucou", "ça va ?", "Chocolatine"};
  wrt::WriteTo(
      stdout,
      wrt::fmt::Join(
          "\n==================================================\n",
          1,
          wrt::fmt::AsRange(toto, " - "),
          wrt::fmt::AsHex(0xCAFEDECA),
          wrt::fmt::AsOct(0xCAFEDECA),
          "Coucou",
          wrt::fmt::AsOct<std::int8_t>(0x33),
          wrt::fmt::AsOct<std::int16_t>(0x33),
          wrt::fmt::Join(" | ",
                         std::array{4783214, 358120835, 2143, 623612},
                         wrt::fmt::AsHex_t{})));

  return 0;
}
