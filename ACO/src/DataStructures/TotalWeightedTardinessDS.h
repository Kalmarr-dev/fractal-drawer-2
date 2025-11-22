#pragma once

#include <vector>

struct Job {
  int duration;
  int deadline;
  int tardiness_cost;
};

struct TotalWeightedTardinessDS
{
  std::vector<Job> jobs;
};
