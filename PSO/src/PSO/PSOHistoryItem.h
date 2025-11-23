#pragma once

#include <vector>
#include <utility>

#include <PSO/Particle.h>

struct PSOHistoryItem
{
  std::vector<Particle> particles;
  std::pair<double, double> global_best;
};
