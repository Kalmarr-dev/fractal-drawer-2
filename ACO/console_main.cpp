#include <iostream>

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

  ACO aco(ds);
  auto aco_solution = aco.solve(50, 10, 100000, 0.95);
  int aco_score = Scorer::score(ds, aco_solution);

  std::cout << bf_score << '\n';
  for (auto &&i : bf_solution)
  {
    std::cout << i << ' ';
  }
  std::cout << '\n';

  std::cout << aco_score << '\n';
  for (auto &&i : aco_solution)
  {
    std::cout << i << ' ';
  }
  std::cout << '\n';

  return 0;
}
