#pragma once

#include <functional>
#include <utility>

#include <PSO/PSOHistory.h>

class PSO
{
private:
  std::function<double(double, double)> target_function;
  std::pair<double, double> lower_margin;
  std::pair<double, double> higher_margin;
  PSOHistory history;
public:
  PSO(const std::function<double(double, double)>& target_function, std::pair<double, double> lower_margin, std::pair<double, double> higher_margin);
  ~PSO() = default;

  // stops when velocities stop
  std::pair<double, double> solve(int particle_amount, double inertia, double acceleration_coefficient, double v_max);
  PSOHistory get_history();
};