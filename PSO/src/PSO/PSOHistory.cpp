#include <PSO/PSOHistory.h>

void PSOHistory::add_iteration(
  const std::vector<Particle> particles,
  const std::pair<double, double> global_best
) {
  this->number_of_iterations++;
  this->items.push_back(PSOHistoryItem{particles, global_best});
}

PSOHistoryItem PSOHistory::get_iteration(int iteration) {
  return items[iteration];
}

int PSOHistory::size() {
  return number_of_iterations;
}