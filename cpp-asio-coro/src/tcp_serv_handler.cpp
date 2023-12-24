#include "pch.hpp"

namespace tcp_serv
{
using boost::asio::awaitable;
using boost::asio::co_spawn;
using boost::asio::detached;
using boost::asio::use_awaitable;
using boost::asio::ip::tcp;

awaitable<void>
handler (tcp::socket socket)
{
  try
    {
      char data[1024];
      for (;;)
        {
          std::size_t n = co_await socket.async_read_some (
              boost::asio::buffer (data), use_awaitable);
          co_await async_write (socket, boost::asio::buffer (data, n),
                                use_awaitable);
        }
    }
  catch (std::exception &e)
    {
      std::printf ("echo Exception: %s\n", e.what ());
    }
}
};
