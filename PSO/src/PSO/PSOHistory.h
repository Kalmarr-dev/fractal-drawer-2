#pragma once

#include <vector>

#include <PSO/PSOHistoryItem.h>

class PSOHistory
{
private:
  int number_of_iterations = 0;
  std::vector<PSOHistoryItem> items;
public:
  PSOHistory() = default;
  ~PSOHistory() = default;

  void add_iteration(
    const std::vector<Particle> particles,
    const std::pair<double, double> global_best
  );
  PSOHistoryItem get_iteration(int iteration);
  int size();
};
