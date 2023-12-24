#include "pch.hpp"

namespace tcp_serv
{
namespace po = boost::program_options;

bool
parse_prog_args (const int argc, const char **argv, std::string &host,
                 boost::asio::ip::port_type &port)
{

  po::options_description desc ("Allowed options");

  desc.add_options ()                                                  //
      ("help,h", "print usage message")                                //
      ("host,h", po::value (&host)->default_value ("0.0.0.0"), "Host") //
      ("port,p", po::value (&port)->default_value (1234), "Port");

  // Parse command line arguments
  po::variables_map vm;
  po::store (po::command_line_parser (argc, argv).options (desc).run (), vm);
  po::notify (vm);

  // Check if there are enough args or if --help is given
  if (vm.count ("help"))
    {
      std::cerr << desc << "\n";
      return false;
    }

  return true;
}
};
