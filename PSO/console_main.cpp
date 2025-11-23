#include <iostream>
#include <array>
#include <math.h>

#include <PSO/PSO.h>

int main(int argc, char const *argv[])
{
  srand(time(0));

  std::function<double(double, double)> target_function = [](double x, double y){
    return (x * x + y - 11) * (x * x + y - 11) + (x + y * y - 7) * (x + y * y - 7);
  };

  std::array<int, 4> particle_amounts = {10, 30, 50, 70};
  std::array<double, 3> inertia_values = {0.4, 0.5, 0.6};
  std::array<double, 4> acceleration_coefficients = {1, 1.5, 2, 2.5};
  double v_max = 5;
  int iterations = 10;
  
  double best_pso_value = 100000;
  std::vector<double> iterations_with_best_pso_value; // for mean/stderr

  for (auto &&particle_amount : particle_amounts)
  {
    for (auto &&inertia : inertia_values)
    {
      for (auto &&acceleration_coefficient : acceleration_coefficients)
      {
        std::vector<double> iteration_values;
        iteration_values.reserve(iterations);
        bool new_best = false;
        for (int iteration = 0; iteration < iterations; iteration++)
        {
          // TODO: find time of best

          PSO pso(target_function, {-6, -6}, {6, 6});
          auto pso_solution = pso.solve(particle_amount, inertia, acceleration_coefficient, v_max);
          double pso_value = target_function(pso_solution.first, pso_solution.second);
          
          iteration_values.push_back(pso_value);

          if (pso_value < best_pso_value)
          {
            new_best = true;
            best_pso_value = pso_value;
            std::cout << "New best! Particles - " << particle_amount << ", inertia - " << inertia << ", acceleration_coefficient - " << acceleration_coefficient << ", value - " << pso_value << '\n';
          }
        }
        if (new_best)
        {
          iterations_with_best_pso_value = std::move(iteration_values);
        }
      }
    }
  }
  

  std::cout << "Results:" << '\n';
  std::cout << best_pso_value << '\n';
  double sum = 0;
  for (auto &&value : iterations_with_best_pso_value)
  {
    sum += value;
  }
  double mean = sum / iterations;
  double deviation_sum = 0;
  for (auto &&value : iterations_with_best_pso_value)
  {
    deviation_sum += (value - mean) * (value - mean);
  }
  double standard_deviation = std::sqrt(deviation_sum / iterations);
  std::cout << "10 iterations: mean - " << mean << " stddev - " << standard_deviation << '\n';

  return 0;
}
