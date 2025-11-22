#pragma once

#include <vector>

struct ACOHistoryItem
{
  std::vector< std::vector<double> > pheromone_matrix;
  std::vector<int> best_permutation;
  std::vector<int> job_tardiness;
};
