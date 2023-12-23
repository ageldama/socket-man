#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <string>

#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

#include "pokemon.hpp"

namespace pokemon::server
{

using boost::asio::ip::tcp;

class session : public std::enable_shared_from_this<session>
{
public:
  session (tcp::socket socket) : socket_ (std::move (socket))
  {
    this->logger = spdlog::stdout_color_mt ("pokemon::server::session");
    this->logger->info ("Accepted");

    this->timer = std::make_unique<boost::asio::deadline_timer> (
        this->socket_.get_executor (), boost::posix_time::seconds (5));
  }

  ~session () { this->logger->info ("Closed"); }

  void
  start ()
  {
    while (true)
      {
        do_write (pokemon::random_pick () + "\n");

        (*this->timer).async_wait ();
      }
  }

private:
  void
  do_write (const std::string data)
  {
    auto self (shared_from_this ());
    boost::asio::async_write (
        socket_, boost::asio::buffer (data, data.size ()),
        [this, self] (boost::system::error_code ec, std::size_t n_written) {
          if (ec)
            {
              // TODO error-handling
              this->logger->warn ("write-error");
            }
        });
  }

  tcp::socket socket_;

  enum
  {
    max_length = 1024
  };

  char data_[max_length];

  std::shared_ptr<spdlog::logger> logger;

  std::unique_ptr<boost::asio::deadline_timer> timer;
};

// TODO sleep?
// TODO on-close?
class server
{
public:
  server (boost::asio::io_context &io_context, short port)
      : acceptor_ (io_context, tcp::endpoint (tcp::v4 (), port))
  {
    this->logger = spdlog::stdout_color_mt ("pokemon::server::server");
    this->logger->info ("Accepting:"); // TODO: host:port?
    do_accept ();
  }

private:
  void
  do_accept ()
  {
    acceptor_.async_accept (
        [this] (boost::system::error_code ec, tcp::socket socket) {
          if (ec)
            {
              // TODO error-handling
            }
          else
            {
              std::make_shared<session> (std::move (socket))->start ();
            }

          do_accept ();
        });
  }

  std::shared_ptr<spdlog::logger> logger;

  tcp::acceptor acceptor_;
};

}; // namespace pokemon::server
