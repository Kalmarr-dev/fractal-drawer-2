#pragma once

#include <fstream>

struct Configuration
{
  int max_number_of_elements_rendered;
  int max_number_of_elements_in_memory;
  int threads;
  Configuration(int argc, char const *argv[]);
  Configuration(std::fstream config_file);
  ~Configuration();
};
