#include <BruteForce/BruteForce.h>

#include <algorithm>
#include <limits>

#include <Scorer/Scorer.h>

BruteForce::BruteForce(TotalWeightedTardinessDS ds) {
  this->ds = ds;
}

std::vector<int> BruteForce::solve() {
  std::vector<int> permutation(ds.jobs.size());

  for (int i = 0; (size_t)i < ds.jobs.size(); i++)
  {
    permutation[i] = i;
  }

  std::vector<int> best_permutation;
  int best_permutation_score = std::numeric_limits<int32_t>::max();

  do {
    int permutation_score = Scorer::score(this->ds, permutation);
    if (permutation_score < best_permutation_score)
    {
      best_permutation = permutation;
      best_permutation_score = permutation_score;
    }
  } while ((std::next_permutation(permutation.begin(), permutation.end())));
  
  return best_permutation;
}