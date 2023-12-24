#pragma once

#include <boost/asio/ip/tcp.hpp>
#include <chrono>
#include <memory>
#include <string>

namespace tcp_serv
{

struct prog_opts
{
  std::string host;
  boost::asio::ip::port_type port;
  int send_interval;
  int keepalive_idle;
  int keepalive_interval;
  int keepalive_count;
};

bool parse_prog_args (const int argc, const char **argv, prog_opts &opts);

};
