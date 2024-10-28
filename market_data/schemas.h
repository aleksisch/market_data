//
// Created by aleksey on 28.10.24.
//

#pragma once

#include <daw/json/daw_json_link.h>
#include <cassert>
#include <cstdint>
#include <string>
#include <utility>

#include "utility.h"

namespace market_data {

struct CoinBaseTicker {
  uint64_t sequence;
  std::string product_id;  // todo: replace with `class Instrument`
  PriceT price;
  PriceT best_bid;
  PriceT best_ask;

  CoinBaseTicker(const std::string& type, uint64_t sequence,
                 std::string product_id, const std::string& price,
                 const std::string& best_bid, const std::string& best_ask)
      : sequence(sequence),
        product_id(std::move(product_id)),
        price(StringToPrice(price)),
        best_bid(StringToPrice(best_bid)),
        best_ask(StringToPrice(best_ask)) {
    assert(type == "ticker");
  }
};

}  // namespace market_data

namespace daw::json {
template <>
struct json_data_contract<market_data::CoinBaseTicker> {
  using type =
      json_member_list<json_string<"type">, json_number<"sequence", uint64_t>,
                       json_string<"product_id">, json_string<"price">,
                       json_string<"best_bid">, json_string<"best_ask">>;
};
}  // namespace daw::json
