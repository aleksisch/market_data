//
// Created by aleksey on 28.10.24.
//

#pragma once

#include <string>
#include <vector>

enum class Exchange {
  CoinBase,
};

struct Config {
  std::string host;
  uint32_t port;
  std::string path;
  std::string ticket_channel;
  std::string subscribe_channel;
};

inline Config getConfig(Exchange exch) {
  switch (exch) {
    case Exchange::CoinBase:
      return Config{
          .host = "ws-feed.exchange.coinbase.com",
          .port = 443,
          .path = "",
          .ticket_channel = "ticker",
          .subscribe_channel = "subscribe",
      };
  }
}
