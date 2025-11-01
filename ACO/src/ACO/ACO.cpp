#include <ACO/ACO.h>

#include <set>
#include <random>

#include <Scorer/Scorer.h>

void ACO::create_ant_path(std::vector<int>* permutation) {
  std::set<int> visited_nodes;
  int current_node = -1;

  while (visited_nodes.size() < ds.jobs.size())
  {
    double weight_sum = 0;
    for (int i = 0; (size_t)i < ds.jobs.size(); i++)
    {
      if (visited_nodes.count(i) == 0)
      {
        double pheromone = pheromone_matrix[current_node + 1][i + 1];
        double weight = pheromone; // TODO add heuristic
        weight_sum += weight;
      }
    }
    int random_value = (rand() % 10000) * 1.0 / 10000.0 * weight_sum;
    int next_node = -1;
    double current_weight_sum = 0;
    for (int i = 0; (size_t)i < ds.jobs.size(); i++)
    {
      if (visited_nodes.count(i) == 0)
      {
        next_node = i;
        double pheromone = pheromone_matrix[current_node + 1][i + 1];
        double weight = pheromone; // TODO add heuristic
        current_weight_sum += weight;
        if (random_value < current_weight_sum)
        {
          break;
        }
      }
    }

    visited_nodes.insert(next_node);
    permutation->push_back(next_node);
    current_node = next_node;
  }
}

void ACO::update_pheromone(const std::vector<int>& permutation, int score) {
  double increment = 1 / score;
  this->pheromone_matrix[0][permutation[0] + 1] += increment;
  for (int i = 0; (size_t)i < permutation.size() - 1; i++)
  {
    this->pheromone_matrix[permutation[i] + 1][permutation[i + 1] + 1] += increment;
  }
}


ACO::ACO(TotalWeightedTardinessDS ds) : ds(ds) {
  int N = ds.jobs.size();
  N++;
  this->pheromone_matrix = std::vector< std::vector<double> >(N, std::vector<double>(N, 1));
}

std::vector<int> ACO::solve(int ants, int max_no_improvement_iterations, int max_itertions, double decay) {
  int no_improvement_iterations = 0;
  int iterations = 0;

  std::vector<int> best_permutation;
  int best_score = __INT32_MAX__;

  while (no_improvement_iterations < max_no_improvement_iterations && iterations < max_itertions)
  {
    std::vector<int> iteration_best_permutation;
    int iteration_best_score = __INT32_MAX__;

    for (int i = 0; i < ants; i++)
    {
      std::vector<int> permutation;
      int score;

      create_ant_path(&permutation);
      score = Scorer::score(this->ds, permutation);
      update_pheromone(permutation, score);

      if (score < iteration_best_score) {
        iteration_best_permutation = permutation;
        iteration_best_score = score;
      }
    }
    
    if (iteration_best_score == best_score)
    {
      no_improvement_iterations++;
    } else if (iteration_best_score < best_score) {
      best_permutation = iteration_best_permutation;
      best_score = iteration_best_score;
      no_improvement_iterations = 0;
    }

    for (int i = 0; (size_t)i < this->pheromone_matrix.size(); i++)
    {
      for (int j = 0; (size_t)j < this->pheromone_matrix[i].size(); j++)
      {
        this->pheromone_matrix[i][j] *= decay;
      }
    }
    this->pheromone_matrix[0][best_permutation[0] + 1] += 1;
    for (int i = 0; (size_t)i < best_permutation.size() - 1; i++)
    {
      this->pheromone_matrix[best_permutation[i] + 1][best_permutation[i + 1] + 1] += 1;
    }

    iterations++;
  }

  return best_permutation;
}