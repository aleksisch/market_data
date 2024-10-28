//
// Created by aleksey on 28.10.24.
//
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/experimental/as_tuple.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/asio/use_future.hpp>

#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>

#include <market_data/coinbase_coro.h>

namespace market_data {

namespace beast = boost::beast;          // from <boost/beast.hpp>
namespace http = beast::http;            // from <boost/beast/http.hpp>
namespace websocket = beast::websocket;  // from <boost/beast/websocket.hpp>
namespace net = boost::asio;             // from <boost/asio.hpp>
namespace ssl = boost::asio::ssl;        // from <boost/asio/ssl.hpp>
using tcp = boost::asio::ip::tcp;        // from <boost/asio/ip/tcp.hpp>

net::awaitable<void> do_session(std::string host, std::string port,
                                const std::string& text,
                                MarketData& marketData) {
    using net::use_awaitable;
    using net::experimental::as_tuple;

    market_data::ssl::context ctx{market_data::ssl::context::tlsv12_client};

    auto ex = co_await net::this_coro::executor;
    // These objects perform our I/O
    tcp::resolver resolver(ex);
    websocket::stream<beast::ssl_stream<beast::tcp_stream>> ws(ex, ctx);

    // Look up the domain name
    auto const results =
        co_await resolver.async_resolve(host, port, use_awaitable);

    // Set a timeout on the operation
    beast::get_lowest_layer(ws).expires_after(std::chrono::seconds(30));

    // Make the connection on the IP address we get from a lookup
    auto ep = co_await beast::get_lowest_layer(ws).async_connect(results,
                                                                 use_awaitable);

    // Set SNI Hostname (many hosts need this to handshake
    // successfully)
    if (!SSL_set_tlsext_host_name(ws.next_layer().native_handle(),
                                  host.c_str())) {
        throw beast::system_error(static_cast<int>(::ERR_get_error()),
                                  net::error::get_ssl_category());
    }

    // Update the host string. This will provide the value of the
    // Host HTTP header during the WebSocket handshake.
    // See https://tools.ietf.org/html/rfc7230#section-5.4
    host += ':' + std::to_string(ep.port());

    // Set a timeout on the operation
    beast::get_lowest_layer(ws).expires_after(std::chrono::seconds(30));

    // Set a decorator to change the User-Agent of the handshake
    ws.set_option(
        websocket::stream_base::decorator([](websocket::request_type& req) {
            req.set(http::field::user_agent,
                    std::string(BOOST_BEAST_VERSION_STRING) +
                        " websocket-client-coro");
        }));

    // Perform the SSL handshake
    co_await ws.next_layer().async_handshake(ssl::stream_base::client,
                                             use_awaitable);

    beast::get_lowest_layer(ws).expires_never();

    ws.set_option(
        websocket::stream_base::timeout::suggested(beast::role_type::client));

    co_await ws.async_handshake(host, "/", use_awaitable);

    co_await ws.async_write(net::buffer(std::string(text)), use_awaitable);

    beast::flat_buffer buffer;

    auto [ec, bytes] = co_await ws.async_read(
        buffer, as_tuple(use_awaitable));  // read response on subscribe
    if (ec) {
        throw beast::system_error(ec);
    }
    buffer.clear();
    while (true) {
        auto [ec, bytes] =
            co_await ws.async_read(buffer, as_tuple(use_awaitable));

        if (ec) {
            if (ec != net::error::eof) {
                throw beast::system_error(ec);
            } else {
                break;
            }
        } else {
            auto ticker = daw::json::from_json<market_data::CoinBaseTicker>(
                std::string_view(static_cast<const char*>(buffer.data().data()),
                                 buffer.size()));
            marketData.onUpdate(ticker);
        }
        buffer.clear();
    }
    co_await ws.async_close(websocket::close_code::normal, use_awaitable);
}
}  // namespace market_data