#pragma once

#include <DataStructures/TotalWeightedTardinessDS.h>
#include <ACO/ACOHistory.h>

class ACO
{
private:
  TotalWeightedTardinessDS ds;
  std::vector< std::vector<double> > pheromone_matrix; // 1-indexed, so size is N+1
  ACOHistory history;

  void create_ant_path(std::vector<int>* permutation);
  void update_pheromone(const std::vector<int>& permutation, int score);
public:
  ACO(TotalWeightedTardinessDS ds);
  ~ACO() = default;

  std::vector<int> solve(int ants, int max_no_improvement_iterations, int max_itertions, double decay);
  ACOHistory get_history();
};
