#include <daw/json/daw_json_link.h>

#include "market_data/Config.h"
#include "market_data/coinbase_coro.h"
#include "market_data/subscriptions.h"

int main() {
  auto config = getConfig(Exchange::CoinBase);
  auto const host = config.host;
  auto const port = std::to_string(config.port);

  net::io_context ioc;

  ssl::context ctx{ssl::context::tlsv12_client};

  auto sub = CoinBaseSubscription{.type = "subscribe",
                                  .product_ids = {"ETH-USD", "BTC-USD"},
                                  .streams = {config.ticket_channel}};

  // We're forced to use exceptions due to boost::beast
  try {
    std::string message = daw::json::to_json(sub);

    net::co_spawn(ioc.get_executor(), do_session(host, port, message, ctx),
                  [&](std::exception_ptr e) {
                    if (e) {
                      std::rethrow_exception(e);
                    }
                  });
    ioc.run();
  } catch (const std::exception& e) {
    std::cerr << "Exception occured: " << e.what() << std::endl;
  }
}