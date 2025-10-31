#pragma once

#include <vector>
#include <list>
// #include "../Renderer/IRendererObserver.h"
#include "../Viewport/IViewportObserver.h"
#include "../helpers/Position.h"
#include "IPointerMoveObserver.h"
#include "IZoomObserver.h"
#include "IWindowResizeObserver.h"
#include "IPointerObserver.h"
#include "IConfirmObserver.h"
#include "IZoomResetObserver.h"
#include "IPointerUpObserver.h"
#include "IClearObserver.h"

template<typename T>
class InputSubject
{
private:
  std::list<IPointerMoveObserver<T>*> pointer_move_observers;
  std::list<IPointerObserver<T>*> primary_button_down_observers;
  std::list<IPointerObserver<T>*> secondary_button_down_observers;
  std::list<IZoomObserver<T>*> zoom_observers;
  std::list<IZoomResetObserver*> zoom_reset_observers;
  std::list<IViewportObserver*> toggle_fullscreen_observers;
  std::list<IClearObserver*> clear_observers;
  std::list<IWindowResizeObserver*> window_resize_observers;
  std::list<IConfirmObserver*> confirm_observers;
  std::list<IPointerUpObserver*> pointer_up_observers;

public:
  InputSubject() = default;
  ~InputSubject() = default;

  void subscribe_to_pointer_move(IPointerMoveObserver<T>* p_observer);
  void unsubscribe_from_pointer_move(IPointerMoveObserver<T>* p_observer);
  void notify_pointer_move(Position<double> pointer);

  void subscribe_to_primary_button_down(IPointerObserver<T>* p_observer);
  void unsubscribe_from_primary_button_down(IPointerObserver<T>* p_observer);
  void notify_primary_button_down(Position<double> pointer);

  void subscribe_to_secondary_button_down(IPointerObserver<T>* p_observer);
  void unsubscribe_from_secondary_button_down(IPointerObserver<T>* p_observer);
  void notify_secondary_button_down(Position<double> pointer);


  void subscribe_to_zoom(IZoomObserver<T>* p_observer);
  void unsubscribe_from_zoom(IZoomObserver<T>* p_observer);
  void notify_zoom(Position<double> pointer, double scale);

  void subscribe_to_zoom_reset(IZoomResetObserver* p_observer);
  void unsubscribe_from_zoom_reset(IZoomResetObserver* p_observer);
  void notify_zoom_reset();

  void subscribe_to_toggle_fullscreen(IViewportObserver* p_observer);
  void unsubscribe_from_toggle_fullscreen(IViewportObserver* p_observer);
  void notify_toggle_fullscreen();

  void subscribe_to_clear(IClearObserver* p_observer);
  void unsubscribe_from_clear(IClearObserver* p_observer);
  void notify_clear();

  void subscribe_to_window_resize(IWindowResizeObserver* p_observer);
  void unsubscribe_from_window_resize(IWindowResizeObserver* p_observer);
  void notify_window_resize(int width, int height);

  void subscribe_to_confirm(IConfirmObserver* p_observer);
  void unsubscribe_from_confirm(IConfirmObserver* p_observer);
  void notify_confirm();

  void subscribe_to_pointer_up(IPointerUpObserver* p_observer);
  void unsubscribe_from_pointer_up(IPointerUpObserver* p_observer);
  void notify_pointer_up();
};
