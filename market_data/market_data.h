//
// Created by aleksey on 28.10.24.
//

#pragma once

#include <dumper/csv_dumper.h>
#include <market_data/schemas.h>
#include <rolling_stats/EMA.h>

#include <fstream>

namespace market_data {

/**
 * Main class to handle market data events
 */
class MarketData {
 public:
  MarketData(std::ofstream&& out) : out_(std::move(out)) {
    dumper::csv_dump(out, "traded_pair", "mid_price", "price", "time",
                     "mid_price_ema", "price_ema");
  }

  template <Ticker T>
  void onUpdate(const T& ticker) {
    auto traded_pair = ticker.getTradedPair();
    if (!midPriceEMA_.count(traded_pair)) {
      midPriceEMA_.emplace(traded_pair, rolling_stats::EMA(WINDOW));
      priceEMA_.emplace(traded_pair, rolling_stats::EMA(WINDOW));
    }
    auto& mid_ema = midPriceEMA_.at(traded_pair);
    auto& price_ema = priceEMA_.at(traded_pair);

    mid_ema.add(ticker.getMidPrice(), ticker.getTime());
    price_ema.add(ticker.getPrice(), ticker.getTime());

    dumper::csv_dump(out_, traded_pair, ticker.getMidPrice(), ticker.getPrice(),
                     ticker.getTime(), mid_ema.get(), price_ema.get());
  }

 private:
  // unordered map for simplicity, however all trading pairs can be enumerated and use faster container e.g. vector
  using EmaT = std::unordered_map<std::string, rolling_stats::EMA>;

  const int WINDOW = 5;

  EmaT midPriceEMA_;
  EmaT priceEMA_;
  std::ofstream out_;
};
}  // namespace market_data