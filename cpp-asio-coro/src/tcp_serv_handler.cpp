#include "pch.hpp"

namespace tcp_serv
{
using boost::asio::awaitable;
using boost::asio::co_spawn;
using boost::asio::detached;
using boost::asio::use_awaitable;
using boost::asio::ip::tcp;

awaitable<void>
handler (tcp::socket socket)
{
  auto logger = spdlog::stdout_color_mt (
      fmt::format ("tcp_serv::handler/{}",
                   utils::endpoint_addr_str (socket.remote_endpoint ())));

  logger->info ("Accepted");

  try
    {
      auto timer = boost::asio::steady_timer{ socket.get_executor () };

      for (;;)
        {
          auto pokemon = pokemon::random_pick ();

          logger->info ("Sending: {}", pokemon);

          co_await async_write (socket, boost::asio::buffer (pokemon + "\n"),
                                use_awaitable);

          timer.expires_after (std::chrono::seconds{ 5 });
          co_await timer.async_wait (use_awaitable);
        }
    }
  catch (std::exception &e)
    {
      logger->error ("Exception: {}", e.what ());
    }
}
};
