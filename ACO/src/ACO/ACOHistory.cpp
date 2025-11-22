#include <ACO/ACOHistory.h>

void ACOHistory::add_iteration(
  const std::vector< std::vector<double> > pheromone_matrix,
  const std::vector<int> best_permutation,
  const std::vector<int> job_tardiness
) {
  this->items.emplace_back(ACOHistoryItem{pheromone_matrix, best_permutation, job_tardiness});
  this->number_of_iterations++;
}

ACOHistoryItem ACOHistory::get_iteration(int iteration) {
  return items[iteration];
}

int ACOHistory::size() {
  return this->number_of_iterations;
}