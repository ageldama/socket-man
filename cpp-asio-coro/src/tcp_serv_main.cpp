#include "pch.hpp"

int
main ()
{
  auto logger = spdlog::stdout_color_mt ("tcp_serv");

  logger->info ("random: {}", pokemon::random_pick ());

  exit (EXIT_SUCCESS);
}
