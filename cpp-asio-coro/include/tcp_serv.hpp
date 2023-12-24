#pragma once

#include <string>

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/write.hpp>

namespace tcp_serv
{
using boost::asio::awaitable;

awaitable<void> listener (const std::string address,
                          const boost::asio::ip::port_type port);

};
