#include "pch.hpp"

namespace tcp_serv
{
using boost::asio::awaitable;
using boost::asio::co_spawn;
using boost::asio::detached;
using boost::asio::use_awaitable;
using boost::asio::ip::tcp;

namespace this_coro = boost::asio::this_coro;

awaitable<void>
listener (const std::string address, const boost::asio::ip::port_type port)
{
  auto logger = spdlog::stdout_color_mt ("tcp_serv::listener");

  auto executor = co_await this_coro::executor;
  tcp::acceptor acceptor (executor);

  boost::asio::ip::tcp::endpoint endpoint{
    boost::asio::ip::address::from_string (address), port
  };

  boost::system::error_code ec;

  acceptor.open (endpoint.protocol (), ec);
  if (ec)
    {
      logger->error ("open error: {}", utils::error_code_str (ec));
      exit (EXIT_FAILURE);
    }

  acceptor.set_option (tcp::acceptor::reuse_address (true), ec);
  if (ec)
    {
      logger->error ("set_option error: {}", utils::error_code_str (ec));
      exit (EXIT_FAILURE);
    }

  acceptor.bind (endpoint, ec);
  if (ec)
    {
      logger->error ("bind error: {}", utils::error_code_str (ec));
      exit (EXIT_FAILURE);
    }

  logger->info ("Accepting: {}",
                utils::endpoint_addr_str (acceptor.local_endpoint ()));

  for (;;)
    {
      tcp::socket socket = co_await acceptor.async_accept (use_awaitable);
      // co_spawn(executor, echo(std::move(socket)), detached);
    }
}

};
