#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <string>

#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/format.hpp>

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

#include "pokemon.hpp"

namespace pokemon::server
{

using boost::asio::ip::tcp;

const unsigned short sending_interval_secs = 5;

std::string
error_code_str (const boost::system::error_code &ec)
{
  return (boost::format ("%1%: %2% %3%") % ec.category ().name () % ec.value ()
          % ec.message ())
      .str ();
}

class session : public std::enable_shared_from_this<session>
{
public:
  session (boost::asio::yield_context yield_context, tcp::socket socket)
      : socket_ (std::move (socket)),
        yield_context_ (std::move (yield_context))
  {

    const std::string addr
        = (boost::format ("%1%:%2%")
           % socket_.remote_endpoint ().address ().to_string ()
           % socket_.remote_endpoint ().port ())
              .str ();

    const std::string logger_name
        = (boost::format ("pokemon::server::session/%1%") % addr).str ();

    this->logger = spdlog::stdout_color_mt (logger_name);
    this->logger->info ("Accepted");
  }

  ~session () { this->logger->info ("Closed"); }

  void
  start ()
  {
    boost::asio::deadline_timer timer (this->socket_.get_executor ());

    while (this->socket_.is_open ())
      {
        const auto pokemon = pokemon::random_pick ();
        this->do_write (pokemon + "\n");

        timer.expires_from_now (
            boost::posix_time::seconds{ sending_interval_secs });
        timer.async_wait (this->yield_context_);
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
              this->logger->warn ("write-error: {}", error_code_str (ec));
              this->socket_.close ();
            }
        });
  }

  tcp::socket socket_;
  boost::asio::yield_context yield_context_;

  enum
  {
    max_length = 1024
  };

  char data_[max_length];

  std::shared_ptr<spdlog::logger> logger;
};

class server
{
public:
  server (boost::asio::io_context &io_context, short port)
      : acceptor_ (io_context, tcp::endpoint (tcp::v4 (), port))
  {
    const std::string addr
        = (boost::format ("%1%:%2%")
           % acceptor_.local_endpoint ().address ().to_string ()
           % acceptor_.local_endpoint ().port ())
              .str ();

    this->logger = spdlog::stdout_color_mt ("pokemon::server::server");
    this->logger->info ("Accepting: {}", addr);
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
              this->logger->warn ("accept-error: {}", error_code_str (ec));
            }
          else
            {
              boost::asio::spawn (
                  this->acceptor_.get_executor (),
                  [this, &socket] (boost::asio::yield_context yield_context) {
                    std::make_shared<session> (std::move (yield_context),
                                               std::move (socket))
                        ->start ();
                  });
            }

          do_accept ();
        });
  }

  std::shared_ptr<spdlog::logger> logger;

  tcp::acceptor acceptor_;
};

}; // namespace pokemon::server
