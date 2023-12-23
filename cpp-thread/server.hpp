#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <boost/asio.hpp>
#include "pokemon.hpp"


namespace pokemon::server {

  using boost::asio::ip::tcp;

class session
  : public std::enable_shared_from_this<session>
{
public:
  session(tcp::socket socket)
    : socket_(std::move(socket))
  {
  }

  void start()
  {
    while (true) {
      do_write(pokemon::random_pick());
    }
  }

private:

  void do_write(const std::string data)
  {
    auto self(shared_from_this());
    boost::asio::async_write(socket_, boost::asio::buffer(data, data.size()),
        [this, self](boost::system::error_code ec, std::size_t n_written)
        {
          if (ec)
          {
            // TODO error-handling
          }
        });
  }

  tcp::socket socket_;
  enum { max_length = 1024 };
  char data_[max_length];
};

class server
{
public:
  server(boost::asio::io_context& io_context, short port)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
  {
    do_accept();
  }

private:
  void do_accept()
  {
    acceptor_.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket)
        {
          if (!ec)
          {
            std::make_shared<session>(std::move(socket))->start();
          }

          do_accept();
        });
  }

  tcp::acceptor acceptor_;
};

};
