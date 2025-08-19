#include "InputSubject.h"

#include <iostream>

template<typename T>
void InputSubject<T>::subscribe_to_primary_button_down(IPointerObserver<T>* p_observer) {
  primary_button_down_observers.push_back(p_observer);
}
template<typename T>
void InputSubject<T>::unsubscribe_from_primary_button_down(IPointerObserver<T>* p_observer) {
  primary_button_down_observers.remove(p_observer);
}
template<typename T>
void InputSubject<T>::notify_primary_button_down(Position<double> pointer) {
  Position<T> t_pointer{T(pointer.x), T(pointer.y)};
  for (auto &&observer : primary_button_down_observers)
  {
    observer->process_primary_click(t_pointer);
  }
}

template<typename T>
void InputSubject<T>::subscribe_to_secondary_button_down(IPointerObserver<T>* p_observer) {
  secondary_button_down_observers.push_back(p_observer);
}
template<typename T>
void InputSubject<T>::unsubscribe_from_secondary_button_down(IPointerObserver<T>* p_observer) {
  secondary_button_down_observers.remove(p_observer);
}
template<typename T>
void InputSubject<T>::notify_secondary_button_down(Position<double> pointer) {
  Position<T> t_pointer{T(pointer.x), T(pointer.y)};
  for (auto &&observer : secondary_button_down_observers)
  {
    observer->process_secondary_click(t_pointer);
  }
}

template<typename T>
void InputSubject<T>::subscribe_to_zoom(IZoomObserver<T>* p_observer) {
  zoom_observers.push_back(p_observer);
}
template<typename T>
void InputSubject<T>::unsubscribe_from_zoom(IZoomObserver<T>* p_observer) {
  zoom_observers.remove(p_observer);
}
template<typename T>
void InputSubject<T>::notify_zoom(Position<double> pointer, double scale) {
  Position<T> t_pointer{T(pointer.x), T(pointer.y)};
  for (auto &&observer : zoom_observers)
  {
    observer->zoom(t_pointer, scale);
  }
}

template<typename T>
void InputSubject<T>::subscribe_to_zoom_reset(IRecursiveRendererObserver<T>* p_observer) {
  zoom_reset_observers.push_back(p_observer);
}
template<typename T>
void InputSubject<T>::unsubscribe_from_zoom_reset(IRecursiveRendererObserver<T>* p_observer) {
  zoom_reset_observers.remove(p_observer);
}
template<typename T>
void InputSubject<T>::notify_zoom_reset() {
  for (auto &&observer : zoom_reset_observers)
  {
    observer->zoom_reset();
  }
}

template<typename T>
void InputSubject<T>::subscribe_to_toggle_fullscreen(IViewportObserver* p_observer) {
  toggle_fullscreen_observers.push_back(p_observer);
}
template<typename T>
void InputSubject<T>::unsubscribe_from_toggle_fullscreen(IViewportObserver* p_observer) {
  toggle_fullscreen_observers.remove(p_observer);
}
template<typename T>
void InputSubject<T>::notify_toggle_fullscreen() {
  for (auto &&observer : toggle_fullscreen_observers)
  {
    observer->set_fullscreen_should_be_toggled();
  }
}

template<typename T>
void InputSubject<T>::subscribe_to_clear(IRecursiveRendererObserver<T>* p_observer) {
  clear_observers.push_back(p_observer);
}
template<typename T>
void InputSubject<T>::unsubscribe_from_clear(IRecursiveRendererObserver<T>* p_observer) {
  clear_observers.remove(p_observer);
}
template<typename T>
void InputSubject<T>::notify_clear() {
  for (auto &&observer : clear_observers)
  {
    observer->clear_shapes();
  }
}

template<typename T>
void InputSubject<T>::subscribe_to_window_resize(IWindowResizeObserver* p_observer) {
  window_resize_observers.push_back(p_observer);
}
template<typename T>
void InputSubject<T>::unsubscribe_from_window_resize(IWindowResizeObserver* p_observer) {
  window_resize_observers.remove(p_observer);
}
template<typename T>
void InputSubject<T>::notify_window_resize(int width, int height) {
  for (auto &&observer : window_resize_observers)
  {
    observer->process_window_resize(width, height);
  }
}