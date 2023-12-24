#pragma once

#include <fstream>
#include <memory>
#include <random>
#include <string>

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

#include <boost/json.hpp>

namespace pokemon
{

class names
{
public:
  names (const std::string filename)
  {
    std::shared_ptr<spdlog::logger> logger
        = spdlog::stdout_color_mt ("pokemon::names");

    std::random_device random_dev;
    this->gen = std::mt19937 (random_dev ());
    std::ifstream f{ filename };
    this->pokemons = (boost::json::parse (f)).as_array ();
    this->dist
        = std::uniform_int_distribution<std::size_t>{ 0,
                                                      pokemons.size () - 1 };

    logger->info ("loaded: {}", filename);
  }

  std::string
  random ()
  {
    auto pick_idx = dist (gen);
    return (pokemons.at (pick_idx)).as_string ().c_str ();
  }

private:
  boost::json::array pokemons;
  std::mt19937 gen;
  std::uniform_int_distribution<std::size_t> dist;
};

std::string
random_pick ()
{
  static names pokemons{ "pokemon-ko.json" };
  return pokemons.random ();
}
};
