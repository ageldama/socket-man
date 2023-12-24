#include "pch.hpp"

using boost::asio::co_spawn;
using boost::asio::detached;

int
main (int argc, const char **argv)
{
  auto logger = spdlog::stdout_color_mt ("tcp_serv");
  std::string host;
  boost::asio::ip::port_type port;

  // logger->info ("random: {}", pokemon::random_pick ());

  tcp_serv::prog_opts opts;

  if (!tcp_serv::parse_prog_args (argc, argv, opts))
    {
      exit (EXIT_FAILURE);
    }

  logger->info ("host={} port={} send_interval={}s", opts.host, opts.port,
                opts.send_interval);

  try
    {
      boost::asio::io_context io_context (1);

      boost::asio::signal_set signals (io_context, SIGINT, SIGTERM);
      signals.async_wait ([&] (auto, auto) { io_context.stop (); });

      co_spawn (io_context, tcp_serv::listener (opts), detached);

      io_context.run ();
    }
  catch (std::exception &e)
    {
      logger->error ("Exception: {}", e.what ());
    }
}
