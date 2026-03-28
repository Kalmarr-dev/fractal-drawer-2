#include <iostream>
#include <fstream>

class Logger
{
public:
  static std::string filename;
  static std::fstream file_stream;

  Logger() = delete;
  ~Logger() = delete;

  static void initialize(std::string filename);
  static void record_query_performance(int shapes_amount, int microseconds);
};
