#pragma once

#include <fstream>
#include <random>
#include <string>
#include "nlohmann/json.hpp"

namespace pokemon {

using json = nlohmann::json;

std::string random_pick() {
  static std::ifstream f("pokemon-ko.json");
  static json pokemons = json::parse(f);

  std::mt19937 gen{std::random_device{}()};
  std::uniform_int_distribution<std::size_t> dist(0, pokemons.size() - 1);

  auto pick_idx = dist(gen);

  return pokemons.at(pick_idx);
}

};
