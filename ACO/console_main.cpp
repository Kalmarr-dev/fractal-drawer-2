#include <iostream>
#include <array>

#include <DataGenerator/DataGenerator.h>
#include <Scorer/Scorer.h>
#include <BruteForce/BruteForce.h>
#include <ACO/ACO.h>

int main(int argc, char const *argv[])
{
  DataGenerator data_generator(time(NULL));
  TotalWeightedTardinessDS ds = data_generator.generate(10, 10, 100);
  
  BruteForce bf(ds);
  auto bf_solution = bf.solve();
  int bf_score = Scorer::score(ds, bf_solution);

  std::array<int, 5> ant_amounts = {10, 20, 30, 40, 50};
  std::array<double, 5> evaporation_coeffs = { 0.99, 0.97, 0.95, 0.93, 0.91 };
  
  std::vector<int> best_aco_solution;
  int best_aco_score = __INT32_MAX__;

  for (auto &&ant_amount : ant_amounts)
  {
    for (auto &&evaporation_coeff : evaporation_coeffs)
    {
      ACO aco(ds);
      auto aco_solution = aco.solve(ant_amount, 10, 100000, evaporation_coeff);
      int aco_score = Scorer::score(ds, aco_solution);

      if (aco_score < best_aco_score)
      {
        best_aco_score = aco_score;
        std::cout << "New best! Ants - " << ant_amount << ", evaporation - " << evaporation_coeff << ", score - " << best_aco_score << '\n';
      }
    }
  }
  

  std::cout << "Results:" << '\n';
  std::cout << bf_score << '\n';
  for (auto &&i : bf_solution)
  {
    std::cout << i << ' ';
  }
  std::cout << '\n';

  std::cout << best_aco_score << '\n';
  for (auto &&i : best_aco_solution)
  {
    std::cout << i << ' ';
  }
  std::cout << '\n';

  return 0;
}
