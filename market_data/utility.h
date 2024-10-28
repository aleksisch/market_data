//
// Created by aleksey on 28.10.24.
//

#pragma once

#include "schemas.h"

namespace market_data {
using PriceT = float;

PriceT StringToPrice(const std::string& price) {
  return std::stod(price);
}
}  // namespace market_data