#pragma once

#include <random>
#include <string>
#include <vector>

namespace pokemon
{

extern const std::vector<std::string> ko_names;

class names
{
public:
  names ();
  std::string random ();

private:
  std::mt19937 gen;
  std::uniform_int_distribution<std::size_t> dist;
};

std::string random_pick ();

};
