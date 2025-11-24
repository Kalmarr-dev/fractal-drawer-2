#include <PSODataProvider/PSODataProvider.h>

#include <math.h>

template<typename T>
PSODataProvider<T>::PSODataProvider(
  const std::function<double(double, double)>& target_function,
  std::pair<double, double> lower_margin, std::pair<double, double> higher_margin,
  int particle_amount, double inertia, double acceleration_coefficient, double v_max
) : target_function(target_function) {
  PSO pso(target_function, lower_margin, higher_margin);
  pso.solve(particle_amount, inertia, acceleration_coefficient, v_max);
  this->history = pso.get_history();
}

template<typename T>
Shapes<T> PSODataProvider<T>::get_shapes_on_camera() const {
  throw "Not Implemented";
}

template<typename T>
Shapes<T> PSODataProvider<T>::get_pso_history(std::vector< Color<T> >* colors) {
  int current_history_index = std::min(this->current_frame * 1.0 / 15.0, (double)this->history.size() - 1);
  auto current_history_item = this->history.get_iteration(current_history_index);

  // TODO using shapes like this causes a memory leak
  Shapes<T> shapes;

  T* offset_0 = new T(0);

  // draw target function 
  std::pair<Position<T>, Position<T>> corners = std::make_pair(Position<T>{-6, -6}, Position<T>{6, 6});
  T camera_width = corners.second.x - corners.first.x;
  T step = camera_width / T(100);
  for (T point_x = corners.first.x; point_x < corners.second.x; point_x = point_x + step)
  {
    for (T point_y = corners.first.y; point_y < corners.second.y; point_y = point_y + step)
    {
      Rectangle<T>* rect = new Rectangle<T>(
        {point_x - step / T(2), point_y - step / T(2)},
        {point_x + step / T(2), point_y + step / T(2)},
        0.5
      );
      shapes.add_shape(rect);
      Color<T> point_color;
      point_color.r = log(target_function(point_x.get_double(offset_0, 0), point_y.get_double(offset_0, 0))) / 10;
      point_color.g = 0.3;
      point_color.b = -log(target_function(point_x.get_double(offset_0, 0), point_y.get_double(offset_0, 0))) / 10;
      colors->push_back(point_color);
    }
  }

  // draw particles
  for (auto &&particle : current_history_item.particles)
  {
    T point_x = T(particle.position.first);
    T point_y = T(particle.position.second);
    Rectangle<T>* rect = new Rectangle<T>(
      {point_x - step / T(2), point_y - step / T(2)},
      {point_x + step / T(2), point_y + step / T(2)},
      0.25
    );
    shapes.add_shape(rect);
    Color<T> point_color(T(1), T(1), T(1));
    colors->push_back(point_color);
  }

  // draw gbest
  T point_x = T(current_history_item.global_best.first);
  T point_y = T(current_history_item.global_best.second);
  Rectangle<T>* rect = new Rectangle<T>(
    {point_x - step / T(2), point_y - step / T(2)},
    {point_x + step / T(2), point_y + step / T(2)},
    0.1
  );
  shapes.add_shape(rect);
  Color<T> point_color(T(1), T(0.3), T(0.3));
  colors->push_back(point_color);

  if (!this->paused)
  {
    this->current_frame++;
  }

  delete offset_0;

  return shapes;
}

template<typename T>
void PSODataProvider<T>::process_confirm() {
  this->paused = !this->paused;
}

template<typename T>
void PSODataProvider<T>::clear_shapes() {
  this->current_frame = 0;
}
