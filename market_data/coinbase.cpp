////
//// Created by aleksey on 28.10.24.
////
//
//#include <boost/beast/core.hpp>
//#include <boost/beast/websocket.hpp>
//#include <boost/asio/strand.hpp>
//#include "coinbase.h"
//
//#include <memory>
//
//#include <boost/beast.hpp>
//#include <iostream>
//#include <string>
//#include <memory>
//
//namespace beast = boost::beast;        // From <boost/beast.hpp>
//namespace http = beast::http;          // From <boost/beast/http.hpp>
//namespace websocket = beast::websocket; // From <boost/beast/websocket.hpp>
//namespace net = boost::asio;           // From <boost/asio.hpp>
//using tcp = boost::asio::ip::tcp;      // From <boost/asio/ip/tcp.hpp>
//
//class Coinbase : public IExchange, std::enable_shared_from_this<Coinbase> {
//public:
//    Coinbase(net::io_context& ioc, Config &&config)
//        : resolver_(ioc), websocket_(ioc), config_(std::move(config)) {
//        do_resolve();
//    }
//
//    void subscribe_ticker() override {
//        // Example subscribe message, format depends on your WebSocket API
//        subscribe(config_.ticket_channel);
//    }
//
//private:
//    void subscribe(const std::string &channel) {
//        std::string message = R"({"action":"subscribe","channel":")" + channel + "\"}";
//        net::post(websocket_.get_executor(), beast::bind_front_handler(&Coinbase::do_write, shared_from_this(), message));
//    }
//
//
//
//    void do_resolve() {
//        resolver_.async_resolve(config_.host, std::to_string(config_.port),
//                                beast::bind_front_handler(&Coinbase::on_resolve, shared_from_this()));
//    }
//
//    void on_resolve(beast::error_code ec, tcp::resolver::results_type results) {
//        if (ec) {
//            std::cerr << "Resolve error: " << ec.message() << std::endl;
//            return;
//        }
//        do_connect(std::move(results));
//    }
//
//    void do_connect(tcp::resolver::results_type results) {
//        beast::get_lowest_layer(websocket_).async_connect(results,
//                                                          beast::bind_front_handler(&Coinbase::on_connect, shared_from_this()));
//    }
//
//    void on_connect(beast::error_code ec, tcp::resolver::results_type::iterator) {
//        if (ec) {
//            std::cerr << "Connect error: " << ec.message() << std::endl;
//            return;
//        }
//        websocket_.async_handshake(config_.host, config_.path,
//                                   beast::bind_front_handler(&Coinbase::on_handshake, shared_from_this()));
//    }
//
//    void on_handshake(beast::error_code ec) {
//        if (ec) {
//            std::cerr << "Handshake error: " << ec.message() << std::endl;
//            return;
//        }
//        do_read();
//    }
//
//    void do_read() {
//        websocket_.async_read(buffer_,
//                              beast::bind_front_handler(&Coinbase::on_read, shared_from_this()));
//    }
//
//    void on_read(beast::error_code ec, std::size_t bytes_transferred) {
//        if (ec) {
//            std::cerr << "Read error: " << ec.message() << std::endl;
//            return;
//        }
//        std::cout << buffer_.data().data() << std::endl;
//        buffer_.consume(bytes_transferred);
//        do_read(); // Continue reading
//    }
//
//    void do_write(const std::string& message) {
//        websocket_.async_write(net::buffer(message),
//                               beast::bind_front_handler(&Coinbase::on_write, shared_from_this()));
//    }
//
//    void on_write(beast::error_code ec, std::size_t) {
//        if (ec) {
//            std::cerr << "Write error: " << ec.message() << std::endl;
//        }
//    }
//
//    tcp::resolver resolver_;
//    websocket::stream<tcp::socket> websocket_;
//    beast::flat_buffer buffer_;
//    Config config_;
//};
//
//std::unique_ptr<IExchange> createCoinBase(net::io_context& ioc, Config config) {
//    return std::make_unique<Coinbase>(ioc, std::move(config));
//}
