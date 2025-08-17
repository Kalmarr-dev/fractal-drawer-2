#include "Configuration.h"

Configuration::Configuration(int argc, char const *argv[]) {
  if (argc < 2)
  {
    throw "Too few command line arguments";
  }
  this->max_number_of_elements_rendered = std::atoi(argv[0]);
  this->max_number_of_elements_in_memory = std::atoi(argv[1]);
}

Configuration::~Configuration() {}