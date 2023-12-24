#include <chrono>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>

#define SPDLOG_FMT_EXTERNAL

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

#include <fmt/core.h>

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/write.hpp>

#include <boost/program_options.hpp>

#include "pokemon.hpp"
#include "progopts.hpp"
#include "tcp_serv.hpp"
#include "utils.hpp"
