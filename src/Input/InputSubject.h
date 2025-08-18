#pragma once

#include <vector>
#include <list>
// #include "../Renderer/IRendererObserver.h"
#include "../Viewport/IViewportObserver.h"
#include "../RecursiveRenderer/IRecursiveRendererObserver.h"
#include "../helpers/Position.h"
#include "IZoomObserver.h"

template<typename T>
class InputSubject
{
private:
  // std::list<> pointer_move_observers;
  // std::list<> primary_button_down_observers;
  // std::list<> secondary_button_down_observers;
  std::list<IZoomObserver<T>*> zoom_observers;
  std::list<IRecursiveRendererObserver<T>*> zoom_reset_observers;
  std::list<IViewportObserver*> toggle_fullscreen_observers;
  std::list<IRecursiveRendererObserver<T>*> clear_observers;
  std::list<IRecursiveRendererObserver<T>*> window_resize_observers;
  
public:
  InputSubject() = default;
  ~InputSubject() = default;

  void subscribe_to_pointer_move() = delete;
  void unsubscribe_from_pointer_move() = delete;
  void notify_pointer_move(Position<double> pointer) = delete;

  // void subscribe_to_primary_button_down();
  // void unsubscribe_from_primary_button_down();
  // void notify_primary_button_down(Position<double> pointer);

  // void subscribe_to_secondary_button_down();
  // void unsubscribe_from_secondary_button_down();
  // void notify_secondary_button_down(Position<double> pointer);


  void subscribe_to_zoom(IZoomObserver<T>* p_observer);
  void unsubscribe_from_zoom(IZoomObserver<T>* p_observer);
  void notify_zoom(Position<double> pointer, double scale);

  void subscribe_to_zoom_reset(IRecursiveRendererObserver<T>* p_observer);
  void unsubscribe_from_zoom_reset(IRecursiveRendererObserver<T>* p_observer);
  void notify_zoom_reset();

  void subscribe_to_toggle_fullscreen(IViewportObserver* p_observer);
  void unsubscribe_from_toggle_fullscreen(IViewportObserver* p_observer);
  void notify_toggle_fullscreen();

  void subscribe_to_clear(IRecursiveRendererObserver<T>* p_observer);
  void unsubscribe_from_clear(IRecursiveRendererObserver<T>* p_observer);
  void notify_clear();

  void subscribe_to_window_resize(IRecursiveRendererObserver<T>* p_observer);
  void unsubscribe_from_window_resize(IRecursiveRendererObserver<T>* p_observer);
  void notify_window_resize(int width, int height);
};
