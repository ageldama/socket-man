#pragma once

#include "pokemon.hpp"
#include <boost/asio.hpp>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

#include <boost/date_time/posix_time/posix_time.hpp>

namespace pokemon::server {

using boost::asio::ip::tcp;

class session : public std::enable_shared_from_this<session> {
public:
  session(tcp::socket socket) : socket_(std::move(socket)) {}

  void start() {
    while (true) {
      do_write(pokemon::random_pick() + "\n");
    }
  }

private:
  void do_write(const std::string data) {
    auto self(shared_from_this());
    boost::asio::async_write(
        socket_, boost::asio::buffer(data, data.size()),
        [this, self](boost::system::error_code ec, std::size_t n_written) {
          if (ec) {
            // TODO error-handling
          }
        });
  }

  tcp::socket socket_;
  enum { max_length = 1024 };
  char data_[max_length];
};

// TODO sleep?
// TODO on-close?
class server {
public:
  server(boost::asio::io_context &io_context, short port)
      : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
    do_accept();
  }

private:
  void do_accept() {
    boost::asio::deadline_timer timer(io, boost::posix_time::seconds(5));

    acceptor_.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket) {
          if (ec) {
            // TODO error-handling
          } else {
            std::make_shared<session>(std::move(socket))->start();
          }

          do_accept();
        });
  }

  tcp::acceptor acceptor_;
};

}; // namespace pokemon::server
