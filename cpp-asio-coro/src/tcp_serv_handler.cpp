#include "pch.hpp"

namespace tcp_serv
{
using boost::asio::awaitable;
using boost::asio::co_spawn;
using boost::asio::detached;
using boost::asio::use_awaitable;
using boost::asio::ip::tcp;

awaitable<void>
handler (tcp::socket socket, const tcp_serv::prog_opts &opts)
{
  auto logger = spdlog::stdout_color_mt (
      fmt::format ("tcp_serv::handler/{}",
                   utils::endpoint_addr_str (socket.remote_endpoint ())));

  std::chrono::seconds interval_secs{ opts.send_interval };

  /*
    KEEPIDLE ..이후 KEEPINTVL 마다, KEEPCNT 횟수만큼 Keep-Alive 확인실패 =>
    종료.

    즉, INTERVAL > KEEPIDLE + (KEEPINTVL x KEEPCNT) ...만큼이어야 keepalive
    유효함.
  */
  socket.set_option (
      boost::asio::detail::socket_option::integer<IPPROTO_TCP, TCP_KEEPIDLE> (
          opts.keepalive_idle));
  socket.set_option (
      boost::asio::detail::socket_option::integer<IPPROTO_TCP, TCP_KEEPINTVL> (
          opts.keepalive_interval));
  socket.set_option (
      boost::asio::detail::socket_option::integer<IPPROTO_TCP, TCP_KEEPCNT> (
          opts.keepalive_count));

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

          timer.expires_after (interval_secs);
          co_await timer.async_wait (use_awaitable);
        }
    }
  catch (std::exception &e)
    {
      logger->error ("Exception: {}", e.what ());
    }
}
};
