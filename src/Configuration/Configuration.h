#pragma once

#include <fstream>

struct Configuration
{
  int max_number_of_elements_rendered;
  int max_number_of_elements_in_memory;
  int threads;
  double minimum_shape_size;
  float camera_change;

  Configuration();
  Configuration(int argc, char const *argv[]);
  Configuration(std::string config_file_path);
  ~Configuration();
};
