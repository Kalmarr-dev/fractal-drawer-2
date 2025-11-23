#pragma once

#include <utility>

struct Particle
{
  std::pair<double, double> position;
  std::pair<double, double> velocity;
  std::pair<double, double> personal_best;
};
