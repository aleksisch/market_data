#include <daw/json/daw_json_link.h>
#include <CLI11/include/CLI/CLI.hpp>

#include <boost/asio.hpp>

#include "market_data/coinbase_coro.h"
#include "market_data/config.h"
#include "market_data/market_data.h"
#include "market_data/subscriptions.h"

namespace net = boost::asio;

int main(int argc, char** argv) {
  auto config = getConfig(Exchange::CoinBase);

  CLI::App app("Coinbase parser to csv");

  std::string file;
  std::vector<std::string> products;
  app.add_option("-f,--file,file", file, "Output filename")
      ->default_val("coinbase.txt");
  app.add_option("--products", products, "Products to subscribe on")
      ->required();

  CLI11_PARSE(app, argc, argv);

  net::io_context ioc;

  std::ofstream out = std::ofstream(file);
  auto marketData = market_data::MarketData(std::move(out));

  auto sub = CoinBaseSubscription{.type = config.subscribe_channel,
                                  .product_ids = std::move(products),
                                  .streams = {config.ticket_channel}};

  std::string message = daw::json::to_json(sub);

  // We're forced to use exceptions due to boost::beast
  try {

    net::co_spawn(
        ioc.get_executor(),
        market_data::do_session(config.host, std::to_string(config.port),
                                message, marketData),
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