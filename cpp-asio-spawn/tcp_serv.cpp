#include "pch.hpp"

int main() {
  auto console = spdlog::stdout_color_mt("console");    

  try
  {
    boost::asio::io_context io_context;
    pokemon::server::server server(io_context, 1234);
    io_context.run();
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return EXIT_SUCCESS;
}
