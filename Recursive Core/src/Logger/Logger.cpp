#include "Logger.h"
#include <thread>
#include <sstream>

std::string Logger::filename;
std::fstream Logger::file_stream;

void Logger::initialize(std::string filename) {
  std::ostringstream filename_stream;
  filename_stream << filename << "." << std::this_thread::get_id() << ".csv";
  Logger::filename = filename_stream.str();
  Logger::file_stream = std::fstream(Logger::filename, std::ios_base::out);
}

void Logger::record_query_performance(int shapes_amount, int microseconds) {
  Logger::file_stream << shapes_amount << "," << microseconds << "\n";
}
