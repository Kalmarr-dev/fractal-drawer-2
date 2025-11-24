#pragma once

#include <Shapes/Shapes.h>
#include <Input/IConfirmObserver.h>
#include <Input/IClearObserver.h>
#include <RendererDataProvider/IRendererDataProvider.h>
#include <helpers/Color.h>
#include <PSO/PSO.h>

template<typename T>
class PSODataProvider : public IRendererDataProvider<T>, public IConfirmObserver, public IClearObserver
{
private:
  std::function<double(double, double)> target_function;
  PSOHistory history;

  std::vector<Position<T>> points;
  bool paused = false;
  int current_frame = 0;
public:
  PSODataProvider(
    const std::function<double(double, double)>& target_function,
    std::pair<double, double> lower_margin, std::pair<double, double> higher_margin,
    int particle_amount, double inertia, double acceleration_coefficient, double v_max
  );
  ~PSODataProvider() = default;

  Shapes<T> get_shapes_on_camera() const;
  Shapes<T> get_pso_history(std::vector< Color<T> >* colors);
  void process_confirm();
  void clear_shapes();
};
