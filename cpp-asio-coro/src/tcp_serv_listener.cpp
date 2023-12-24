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
listener (const tcp_serv::prog_opts &opts
          /*const std::string address, const boost::asio::ip::port_type port*/
)
{
  boost::system::error_code ec;

  auto logger = spdlog::stdout_color_mt ("tcp_serv::listener");
  logger->info ("PID = {}", getpid ());

  boost::asio::ip::tcp::endpoint endpoint{
    boost::asio::ip::address::from_string (opts.host), opts.port
  };

  try
    {
      auto executor = co_await this_coro::executor;
      tcp::acceptor acceptor (executor, endpoint);

      acceptor.set_option (boost::asio::socket_base::keep_alive (true));

      logger->debug ("is_open = {}", acceptor.is_open ());

      logger->info ("Accepting: {}",
                    utils::endpoint_addr_str (acceptor.local_endpoint ()));

      for (;;)
        {
          tcp::socket socket = co_await acceptor.async_accept (use_awaitable);
          co_spawn (executor, tcp_serv::handler (std::move (socket), opts),
                    detached);
        }
    }
  catch (std::exception &e)
    {
      logger->error ("Exception: {}", e.what ());
      exit (EXIT_FAILURE);
    }
}

};
