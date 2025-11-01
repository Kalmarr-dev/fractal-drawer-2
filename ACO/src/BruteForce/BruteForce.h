#pragma once

#include <DataStructures/TotalWeightedTardinessDS.h>

class BruteForce
{
private:
  TotalWeightedTardinessDS ds;
public:
  BruteForce(TotalWeightedTardinessDS ds);
  ~BruteForce() = default;

  std::vector<int> solve();
};
