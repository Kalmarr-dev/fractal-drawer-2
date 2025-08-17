#pragma once

#include "../Renderer/IRendererObserver.h";
#include "../RecursiveRenderer/IRecursiveRendererObserver.h";
#include "../helpers/Position.h";

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

  void subscribe_to_zoom_in(IRecursiveRendererObserver observer);
  void unsubscribe_from_zoom_in(IRecursiveRendererObserver observer);

  void subscribe_to_zoom_out(IRecursiveRendererObserver observer);
  void unsubscribe_from_zoom_out(IRecursiveRendererObserver observer);

  void subscribe_to_zoom_reset(IRecursiveRendererObserver observer);
  void unsubscribe_from_zoom_reset(IRecursiveRendererObserver observer);

  void subscribe_to_toggle_fullscreen(IRendererObserver observer);
  void unsubscribe_from_toggle_fullscreen(IRendererObserver observer);

  void subscribe_to_clear(IRecursiveRendererObserver observer);
  void unsubscribe_from_clear(IRecursiveRendererObserver observer);
};
