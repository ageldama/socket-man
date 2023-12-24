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

awaitable<void> listener (const tcp_serv::prog_opts &opts);

awaitable<void> handler (boost::asio::ip::tcp::socket socket,
                         const tcp_serv::prog_opts &opts);

};
