#include <iostream>
#include <cstdlib>
#include <fstream>

#include "nlohmann/json.hpp"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"


using json = nlohmann::json;


int main() {
  auto console = spdlog::stdout_color_mt("console");    

  std::ifstream f("pokemon-ko.json");
  json pokemons = json::parse(f);

  // console->info("pokemons: {}", pokemons.dump());
  // console->info("pokemons: {}", pokemons.size());

  /*
  for(auto e : pokemons) {
    console->info("pokemon: {}", e);
    }
  */

  return EXIT_SUCCESS;
}
