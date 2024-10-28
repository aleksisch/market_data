#include <boost/asio/awaitable.hpp>

#include <cstdlib>
#include <functional>
#include <iostream>
#include <string>

#include <market_data/market_data.h>
#include <market_data/schemas.h>

namespace market_data {

//------------------------------------------------------------------------------

// Sends a WebSocket message and prints the response
boost::asio::awaitable<void> do_session(std::string host, std::string port,
                                        std::string const& text,
                                        market_data::MarketData& marketData);
}  // namespace market_data