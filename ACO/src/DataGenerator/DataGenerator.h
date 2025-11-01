#pragma once

#include <DataStructures/TotalWeightedTardinessDS.h>

class DataGenerator
{
private:
  int seed;
public:
  DataGenerator(int seed = 42);
  ~DataGenerator() = default;

  TotalWeightedTardinessDS generate(int jobs_amount, int average_duration, int max_cost = 100);
};
