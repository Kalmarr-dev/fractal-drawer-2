#pragma once

#include "../Renderer/IRendererObserver.h";
#include "../RecursiveRenderer/IRecursiveRendererObserver.h";
#include "../helpers/Position.h";

template<typename T>
class InputSubject
{
private:
  Position<double> pointer_position;
public:
  InputSubject();
  ~InputSubject();

  void subscribe_to_pointer_move();
  void unsubscribe_from_pointer_move();

  void subscribe_to_primary_button_down();
  void unsubscribe_from_primary_button_down();

  void subscribe_to_primary_button_up();
  void unsubscribe_from_primary_button_up();

  void subscribe_to_zoom_in(IRecursiveRendererObserver<T> observer);
  void unsubscribe_from_zoom_in(IRecursiveRendererObserver<T> observer);

  void subscribe_to_zoom_out(IRecursiveRendererObserver<T> observer);
  void unsubscribe_from_zoom_out(IRecursiveRendererObserver<T> observer);

  void subscribe_to_zoom_reset(IRecursiveRendererObserver<T> observer);
  void unsubscribe_from_zoom_reset(IRecursiveRendererObserver<T> observer);

  void subscribe_to_toggle_fullscreen(IRendererObserver observer);
  void unsubscribe_from_toggle_fullscreen(IRendererObserver observer);

  void subscribe_to_clear(IRecursiveRendererObserver<T> observer);
  void unsubscribe_from_clear(IRecursiveRendererObserver<T> observer);

  void subscribe_to_window_resize(IRecursiveRendererObserver<T> observer);
  void unsubscribe_from_window_resize(IRecursiveRendererObserver<T> observer);
};
