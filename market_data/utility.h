//
// Created by aleksey on 28.10.24.
//

#pragma once

#include <string>

namespace market_data {

using PriceT = float;
using TimeT = uint64_t;

inline PriceT StringToPrice(const std::string& price) {
  return std::stod(price);
}
}  // namespace market_data