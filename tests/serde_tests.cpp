//
// Created by aleksey on 28.10.24.
//

#include "gtest/gtest.h"

#include <daw/json/daw_json_link.h>

#include "market_data/schemas.h"

TEST(Deserialization, Ticket) {
  std::string json_ticket = R"(
    {
      "type": "ticker",
      "sequence": 37475248783,
      "product_id": "ETH-USD",
      "price": "1285.22",
      "open_24h": "1310.79",
      "volume_24h": "245532.79269678",
      "low_24h": "1280.52",
      "high_24h": "1313.8",
      "volume_30d": "9788783.60117027",
      "best_bid": "1285.04",
      "best_bid_size": "0.46688654",
      "best_ask": "1285.27",
      "best_ask_size": "1.56637040",
      "side": "buy",
      "time": "2022-10-19T23:28:22.061769Z",
      "trade_id": 370843401,
      "last_size": "11.4396987"
    }
    )";
  auto ticker = daw::json::from_json<market_data::CoinBaseTicker>(
      std::string_view(json_ticket));
  ASSERT_EQ(ticker.sequence, 37475248783);
  ASSERT_EQ(ticker.product_id, "ETH-USD");
  ASSERT_FLOAT_EQ(ticker.price, 1285.22);
  ASSERT_FLOAT_EQ(ticker.best_bid, 1285.04);
  ASSERT_FLOAT_EQ(ticker.best_ask, 1285.27);
}