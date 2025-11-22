#pragma once

#include <ACO/ACOHistoryItem.h>

class ACOHistory
{
private:
  int number_of_iterations = 0;
  std::vector<ACOHistoryItem> items;
public:
  ACOHistory() = default;
  ~ACOHistory() = default;

  void add_iteration(
    const std::vector< std::vector<double> > pheromone_matrix,
    const std::vector<int> best_permutation,
    const std::vector<int> job_tardiness
  );
  ACOHistoryItem get_iteration(int iteration);
  int size();
};
