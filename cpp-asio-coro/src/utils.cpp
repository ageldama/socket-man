#include "pch.hpp"

namespace utils
{

std::string
endpoint_addr_str (const boost::asio::ip::tcp::endpoint &endpoint)
{
  return fmt::format ("{}:{}", endpoint.address ().to_string (),
                      endpoint.port ());
}

std::string
error_code_str (const boost::system::error_code &ec)
{
  return fmt::format ("{}: {} {}", ec.category ().name (), ec.value (),
                      ec.message ());
}

};
