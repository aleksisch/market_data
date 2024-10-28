//
// Created by aleksey on 28.10.24.
//

#include <market_data/schemas.h>

#include <chrono>
#include <iomanip>

namespace market_data {

market_data::CoinBaseTicker::CoinBaseTicker(
    const std::string& type, uint64_t sequence, std::string product_id,
    const std::string& price, const std::string& best_bid,
    const std::string& best_ask, const std::string& timestamp)
    : sequence(sequence),
      product_id(std::move(product_id)),
      price(StringToPrice(price)),
      best_bid(StringToPrice(best_bid)),
      best_ask(StringToPrice(best_ask)) {
  assert(type == "ticker");
  // Unfortunately coinbase does not provide time in seconds
  std::istringstream ss(timestamp);
  std::tm tm = {};
  ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");
  if (ss.fail()) {
    throw std::runtime_error("Failed to parse timestamp");
  }
  std::string fraction;
  long long micros = 0LL;
  if (ss >> fraction) {
    fraction = fraction.substr(1, fraction.size() - 2);  // .123Z => 123
    // Convert the fractional seconds to milliseconds
    micros = std::stoll(fraction);
    assert(micros < 1 '000' 000);
    micros %= 1 '000' 000;
  }
  auto duration = std::chrono::system_clock::from_time_t(std::mktime(&tm))
                      .time_since_epoch() +
                  std::chrono::microseconds(micros);
  time =
      std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
}
}  // namespace market_data