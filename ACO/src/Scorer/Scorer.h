#pragma once

#include <vector>

#include <DataStructures/TotalWeightedTardinessDS.h>

namespace Scorer
{
  int score(const TotalWeightedTardinessDS& ds, const std::vector<int>& permutation);
}
