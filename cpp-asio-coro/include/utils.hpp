#pragma once

#include <boost/asio/ip/tcp.hpp>
#include <string>

namespace utils
{
std::string endpoint_addr_str (const boost::asio::ip::tcp::endpoint &endpoint);

std::string error_code_str (const boost::system::error_code &ec);

};
