#include <Scorer/Scorer.h>

int Scorer::score(const TotalWeightedTardinessDS& ds, const std::vector<int>& permutation) {
  int current_time = 0;
  int score = 0;

  for (auto &&job_i : permutation)
  {
    current_time += ds.jobs[job_i].duration;
    int overtime = current_time - ds.jobs[job_i].deadline;
    if (overtime > 0)
    {
      score += overtime * ds.jobs[job_i].tardiness_cost;
    }
  }

  return score;
}