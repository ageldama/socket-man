#pragma once

#include <boost/asio/ip/tcp.hpp>
#include <string>

namespace tcp_serv
{
bool parse_prog_args (const int argc, const char **argv, std::string &host,
                      boost::asio::ip::port_type &port);

};
