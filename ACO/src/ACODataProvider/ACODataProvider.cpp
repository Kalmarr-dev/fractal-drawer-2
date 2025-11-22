#include <ACODataProvider/ACODataProvider.h>

#include <iostream>
#include <math.h>

#include <Shapes/Line.h>

template<typename T>
ACODataProvider<T>::ACODataProvider(
  int jobs_amount, int average_duration, int max_cost,
  int ants, int max_no_improvement_iterations, int max_itertions, double decay
) {
  DataGenerator data_generator(time(0));
  ds = data_generator.generate(jobs_amount, average_duration, max_cost);
  
  BruteForce bf(ds);
  auto bf_solution = bf.solve();
  int bf_score = Scorer::score(ds, bf_solution);

  ACO aco(ds);
  auto aco_solution = aco.solve(ants, max_no_improvement_iterations, max_itertions, decay);
  int aco_score = Scorer::score(ds, aco_solution);
  ACOHistory history = aco.get_history();

  this->bf_score = bf_score;
  this->aco_score = aco_score;
  this->bf_solution = bf_solution;
  this->aco_solution = aco_solution;
  this->history = history;

  for (int i = 0; i < jobs_amount; i++)
  {
    Position<T> a;
    a.x = T((rand() % 100) * 2.0 / 100.0 - 1.0);
    a.y = T((rand() % 100) * 2.0 / 100.0 - 1.0);
    this->points.push_back(a);
  }
}

template<typename T>
void ACODataProvider<T>::print_scores() {
  std::cout << "Best solution: " << bf_score << '\n';
  std::cout << "ACO solution: " << aco_score << '\n';
}

template<typename T>
Shapes<T> ACODataProvider<T>::get_shapes_on_camera() const {
  return Shapes<T>();
}

template<typename T>
Shapes<T> ACODataProvider<T>::get_aco_history(std::vector< Color<T> >* colors) {
  int current_history_index = std::min(this->current_frame * 1.0 / 15.0, (double)this->history.size() - 1);
  auto current_history_item = this->history.get_iteration(current_history_index);


  // TODO using shapes like this causes a memory leak
  Shapes<T> shapes;

  for (int i = 1; (size_t)i < current_history_item.pheromone_matrix.size(); i++)
  {
    for (int j = 1; (size_t)j < current_history_item.pheromone_matrix[i].size(); j++)
    {
      if (i == j)
      {
        continue;
      }

      Position<T> p1 = this->points[i - 1];
      Position<T> p2 = this->points[j - 1];
      Line<T>* l = new Line<T>(
        Position<T>(T(p1.x), T(p1.y)), Position<T>(T(p2.x), T(p2.y))
      );
      l->set_depth(0.2);
      shapes.add_shape(l);

      double tint = log(current_history_item.pheromone_matrix[i][j] + 1.0) / 2.0;
      colors->push_back(Color<T>(T(tint), T(0.5 * tint), T(0.3 * tint)));
    }
  }
  

  for (int i = 0; (size_t)i < this->ds.jobs.size(); i++)
  {
    Position<T> p = this->points[i];
    double size = log(this->ds.jobs[i].duration + 1.0) / 100.0;
    Rectangle<T>* r = new Rectangle<T>(
      Position<T>(T(p.x) - T(size), T(p.y) - T(size)), Position<T>(T(p.x) + T(size), T(p.y) + T(size)), 0.0
    );
    shapes.add_shape(r);

    double tint = current_history_item.job_tardiness[i] / 100.0;
    colors->push_back(Color<T>(T(0.5), T(0.3), T(1.0 - tint)));
  }

  if (current_history_index == this->history.size() - 1)
  {
    for (int i = 1; (size_t)i < current_history_item.best_permutation.size(); i++)
    {
      Position<T> p1 = this->points[current_history_item.best_permutation[i - 1]];
      Position<T> p2 = this->points[current_history_item.best_permutation[i]];
      Line<T>* l = new Line<T>(
        Position<T>(T(p1.x), T(p1.y)), Position<T>(T(p2.x), T(p2.y))
      );
      l->set_depth(0.1);
      shapes.add_shape(l);
      colors->push_back(Color<T>(T(0.8), T(0.8), T(1.0)));
    }
  }

  if (!this->paused)
  {
    this->current_frame++;
  }

  return shapes;
}

template<typename T>
void ACODataProvider<T>::process_confirm() {
  this->paused = !this->paused;
}

template<typename T>
void ACODataProvider<T>::clear_shapes() {
  this->current_frame = 0;
}
