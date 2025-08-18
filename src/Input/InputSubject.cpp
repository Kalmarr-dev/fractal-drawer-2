#include "InputSubject.h"

// void InputSubject<T>::subscribe_to_primary_button_down()
// void InputSubject<T>::unsubscribe_from_primary_button_down()
// void InputSubject<T>::notify_primary_button_down(Position<double> pointer)

// void InputSubject<T>::subscribe_to_secondary_button_down()
// void InputSubject<T>::unsubscribe_from_secondary_button_down()
// void InputSubject<T>::notify_secondary_button_down(Position<double> pointer)

template<typename T>
void InputSubject<T>::subscribe_to_zoom_in(IRecursiveRendererObserver<T>* p_observer) {
  zoom_in_observers.push_back(p_observer);
}
template<typename T>
void InputSubject<T>::unsubscribe_from_zoom_in(IRecursiveRendererObserver<T>* p_observer) {
  zoom_in_observers.remove(p_observer);
}
template<typename T>
void InputSubject<T>::notify_zoom_in(Position<double> pointer, double scale) {
  Position<T> t_pointer{T(pointer.x), T(pointer.y)};
  for (auto &&observer : zoom_in_observers)
  {
    observer->zoom_in(t_pointer, scale);
  }
}

template<typename T>
void InputSubject<T>::subscribe_to_zoom_out(IRecursiveRendererObserver<T>* p_observer) {
  zoom_out_observers.push_back(p_observer);
}
template<typename T>
void InputSubject<T>::unsubscribe_from_zoom_out(IRecursiveRendererObserver<T>* p_observer) {
  zoom_out_observers.remove(p_observer);
}
template<typename T>
void InputSubject<T>::notify_zoom_out(Position<double> pointer, double scale) {
  Position<T> t_pointer{T(pointer.x), T(pointer.y)};
  for (auto &&observer : zoom_out_observers)
  {
    observer->zoom_out(t_pointer, scale);
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
void InputSubject<T>::subscribe_to_toggle_fullscreen(IRendererObserver* p_observer) {
  toggle_fullscreen_observers.push_back(p_observer);
}
template<typename T>
void InputSubject<T>::unsubscribe_from_toggle_fullscreen(IRendererObserver* p_observer) {
  toggle_fullscreen_observers.remove(p_observer);
}
template<typename T>
void InputSubject<T>::notify_toggle_fullscreen() {
  for (auto &&observer : toggle_fullscreen_observers)
  {
    observer->toggle_fullscreen();
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
void InputSubject<T>::subscribe_to_window_resize(IRecursiveRendererObserver<T>* p_observer) {
  window_resize_observers.push_back(p_observer);
}
template<typename T>
void InputSubject<T>::unsubscribe_from_window_resize(IRecursiveRendererObserver<T>* p_observer) {
  window_resize_observers.remove(p_observer);
}
template<typename T>
void InputSubject<T>::notify_window_resize(int width, int height) {
  for (auto &&observer : window_resize_observers)
  {
    observer->resize_camera(width, height);
  }
}