#include <DataGenerator/DataGenerator.h>

#include <stdlib.h>

DataGenerator::DataGenerator(int seed) {
  this->seed = seed;
  srand(seed);
}

TotalWeightedTardinessDS DataGenerator::generate(int jobs_amount, int average_duration, int max_cost) {
  int total_time = 0;
  std::vector<int> durations(jobs_amount);

  for (int i = 0; i < jobs_amount; i++)
  {
    durations[i] = 1 + rand() % (average_duration * 2 - 1);
    total_time += durations[i];
  }
  
  TotalWeightedTardinessDS ds;
  ds.jobs.resize(jobs_amount);

  for (int i = 0; i < jobs_amount; i++)
  {
    ds.jobs[i].duration = durations[i];
    ds.jobs[i].deadline = rand() % total_time;
    ds.jobs[i].tardiness_cost = rand() % max_cost;
  }

  return ds;
}

