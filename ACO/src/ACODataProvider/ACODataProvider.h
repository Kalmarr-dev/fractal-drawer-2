#pragma once

#include <Shapes/Shapes.h>
#include <Input/IConfirmObserver.h>
#include <Input/IClearObserver.h>
#include <RendererDataProvider/IRendererDataProvider.h>
#include <ACO/ACOHistory.h>
#include <DataGenerator/DataGenerator.h>
#include <Scorer/Scorer.h>
#include <BruteForce/BruteForce.h>
#include <ACO/ACO.h>
#include <helpers/Color.h>

template<typename T>
class ACODataProvider : public IRendererDataProvider<T>, public IConfirmObserver, public IClearObserver
{
private:
  TotalWeightedTardinessDS ds;
  int bf_score;
  int aco_score;
  std::vector<int> bf_solution;
  std::vector<int> aco_solution;
  ACOHistory history;

  std::vector<Position<T>> points;
  bool paused = false;
  int current_frame = 0;
public:
  ACODataProvider(
    int jobs_amount, int average_duration, int max_cost,
    int ants, int max_no_improvement_iterations, int max_itertions, double decay
  );
  ~ACODataProvider() = default;

  void print_scores();
  Shapes<T> get_shapes_on_camera() const;
  Shapes<T> get_aco_history(std::vector< Color<T> >* colors);
  void process_confirm();
  void clear_shapes();
};
