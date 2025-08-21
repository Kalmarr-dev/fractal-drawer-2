#pragma once

#include <GLFW/glfw3.h>
#include "GLFWKey.h"
#include "../InputSubject.h"
#include "../../helpers/Position.h"
#include "../../Viewport/IViewport.h"

template<typename T>
class GLFWInput : public InputSubject<T>, public IInputObserver
{
private:
  IViewport* p_viewport;
  GLFWKey fullscreen_key;
  GLFWKey zoom_reset_key;
  GLFWKey zoom_in_key;
  GLFWKey zoom_out_key;
  GLFWKey clear_key;
  GLFWKey lock_zoom_key;
  GLFWKey confirm_key;

  void on_click_callback(GLFWwindow* window, int button, int action, int mods);
  void on_pointer_move_callback(GLFWwindow* window, double x, double y);
  void on_key_press_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
  void on_window_resize_callback(GLFWwindow* window, int width, int height);
public:
  GLFWInput(IViewport* p_viewport, GLFWKey fullscreen_key, GLFWKey zoom_reset_key, GLFWKey zoom_in_key, GLFWKey zoom_out_key, GLFWKey clear_key, GLFWKey lock_zoom_key, GLFWKey confirm_key);
  ~GLFWInput() = default;
  void subscribe_viewport_to_callbacks(IViewport* p_viewport) override;
};
