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

template <class T>
concept Ticker = requires(T const t) {
    { t.getMidPrice() } -> std::same_as<PriceT>;
    { t.getPrice() } -> std::same_as<PriceT>;
    { t.getTradedPair() } -> std::same_as<std::string>;
    { t.getTime() } -> std::same_as<TimeT>;
};

struct CoinBaseTicker {
    uint64_t sequence;
    std::string product_id;  // todo: replace with `class Instrument`
    TimeT time;
    PriceT price;
    PriceT best_bid;
    PriceT best_ask;

    CoinBaseTicker(const std::string& type, uint64_t sequence,
                   std::string product_id, const std::string& price,
                   const std::string& best_bid, const std::string& best_ask,
                   const std::string& timestamp);

    PriceT getMidPrice() const { return (best_bid + best_ask) / 2; }

    PriceT getPrice() const { return price; }

    std::string getTradedPair() const { return product_id; }

    TimeT getTime() const { return time; }
};

}  // namespace market_data

namespace daw::json {
template <>
struct json_data_contract<market_data::CoinBaseTicker> {
    using type =
        json_member_list<json_string<"type">, json_number<"sequence", uint64_t>,
                         json_string<"product_id">, json_string<"price">,
                         json_string<"best_bid">, json_string<"best_ask">,
                         json_string<"time">>;
};
}  // namespace daw::json
