#include "pch.hpp"

namespace tcp_serv
{
namespace po = boost::program_options;

bool
parse_prog_args (const int argc, const char **argv, prog_opts &opts)
{

  po::options_description desc ("Allowed options");

  desc.add_options ()                   //
      ("help,h", "print usage message") //
      ("host,h", po::value (&(opts.host))->default_value ("0.0.0.0"),
       "Host")                                                           //
      ("port,p", po::value (&(opts.port))->default_value (1234), "Port") //
      ("interval,i", po::value (&(opts.send_interval))->default_value (7),
       "Interval") //
      ("keepalive-idle", po::value (&(opts.keepalive_idle))->default_value (1),
       "KeepAlive Idle") //
      ("keepalive-interval",
       po::value (&(opts.keepalive_interval))->default_value (1),
       "KeepAlive Interval") //
      ("keepalive-count",
       po::value (&(opts.keepalive_count))->default_value (2),
       "KeepAlive Count") //
      ;

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
