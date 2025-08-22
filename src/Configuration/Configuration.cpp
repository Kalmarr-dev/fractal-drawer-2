#include "Configuration.h"

#include <iostream>

Configuration::Configuration() {
  this->max_number_of_elements_rendered = 0;
  this->max_number_of_elements_in_memory = 0;
  this->threads = 0;
  this->minimum_shape_size = 0;
  this->camera_change = 0;
}

Configuration::Configuration(int argc, char const *argv[]) {
  if (argc == 1)
  {
    throw std::runtime_error("No command line arguments");
  }
  if (argc > 1 && argc < 6)
  {
    std::cerr << "Command line arguments should be as described in config.txt" << std::endl;
    throw std::runtime_error("Too few command line arguments");
  }
  this->max_number_of_elements_rendered = std::atoi(argv[1]);
  this->max_number_of_elements_in_memory = std::atoi(argv[2]);
  this->threads = std::atoi(argv[3]);
  this->minimum_shape_size = std::atof(argv[4]);
  this->camera_change = std::atof(argv[5]);
}

Configuration::Configuration(std::string config_file_path) {
  std::ifstream config_file(config_file_path);
  if (!config_file.good())
  {
    throw std::runtime_error("No config file at " + config_file_path);
  }

  int current_line = 0;
  std::string line;
  while (std::getline(config_file, line)) {
    if (line.find("#") != 0) {
      switch (current_line)
      {
      case 0:
        this->max_number_of_elements_rendered = std::atoi(line.c_str());
        break;
      case 1:
        this->max_number_of_elements_in_memory = std::atoi(line.c_str());
        break;
      case 2:
        this->threads = std::atoi(line.c_str());
        break;
      case 3:
        this->minimum_shape_size = std::atof(line.c_str());
        break;
      case 4:
        this->camera_change = std::atof(line.c_str());
        break;
      default:
        break;
      }
      current_line++;
    }
  }

  if (current_line < 5)
  {
    throw std::runtime_error("Config file is missing some opotions");
  }
}

Configuration::~Configuration() {}