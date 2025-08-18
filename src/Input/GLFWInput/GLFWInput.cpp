#include "GLFWInput.h"

#include <utility>
#include "../../Viewport/GLFWViewport/GLFWViewport.h"

template<typename T>
GLFWInput<T>::GLFWInput(IViewport* p_viewport, GLFWKey fullscreen_key, GLFWKey zoom_reset_key, 
  GLFWKey zoom_in_key, GLFWKey zoom_out_key, GLFWKey clear_key, GLFWKey lock_zoom_key
) : p_viewport(p_viewport), fullscreen_key(fullscreen_key), zoom_reset_key(zoom_reset_key),
    zoom_in_key(zoom_in_key), zoom_out_key(zoom_out_key), clear_key(clear_key), lock_zoom_key(lock_zoom_key) 
{}

template<typename T>
void GLFWInput<T>::on_click_callback(GLFWwindow* window, int button, int action, int mods) {
  double mouseX, mouseY;
  glfwGetCursorPos(window, &mouseX, &mouseY);
  int windowW, windowH;
  glfwGetWindowSize(window, &windowW, &windowH);

  if (action == GLFW_PRESS) {
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
      // this->notify_secondary_button_down(Position<double>{mouseX / windowW, (- mouseY / windowH + 1.0)});
    } else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
      // TODO
      throw "Not implemented";
    } else {
      // this->notify_primary_button_down(Position<double>{mouseX / windowW, (- mouseY / windowH + 1.0)});
    }
  }
}
// template<typename T>
// void GLFWInput<T>::on_pointer_move(Position<double> position) {
//   void notify_pointer_move(Position<double> pointer);
// }

template<typename T>
void GLFWInput<T>::on_key_press_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  double mouseX, mouseY;
  glfwGetCursorPos(window, &mouseX, &mouseY);
  int windowW, windowH;
  glfwGetWindowSize(window, &windowW, &windowH);
  if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
    // TODO draw fractal
    throw "Not implemented";
  }
  if (key == clear_key.code && action == GLFW_PRESS) {
    this->notify_clear();
  }
  if (key == zoom_reset_key.code && action == GLFW_PRESS) {
    this->notify_zoom_reset();
  }
  if (key == zoom_out_key.code && action != GLFW_RELEASE) {
    this->notify_zoom(Position<double>{mouseX / windowW, (- mouseY / windowH + 1.0)}, 0.995);
  }
  if (key == zoom_in_key.code && action != GLFW_RELEASE) {
    this->notify_zoom(Position<double>{mouseX / windowW, (- mouseY / windowH + 1.0)}, 1.0055555555);
  }
  if (key == lock_zoom_key.code && action == GLFW_PRESS) {
    // TODO lock zoom
    lock_zoom_key.amount_of_times_clicked++;
    throw "Lock zoom not implemented";
  }
  if (key == fullscreen_key.code && action == GLFW_PRESS) {
    this->notify_toggle_fullscreen();
    std::pair<int, int> size = this->p_viewport->get_size();
    this->notify_window_resize(size.first, size.second);
  }
}

template<typename T>
void GLFWInput<T>::on_window_resize_callback(GLFWwindow* window, int width, int height)
{
  this->notify_window_resize(width, height);
}

template<typename T>
void GLFWInput<T>::subscribe_viewport_to_callbacks(IViewport* p_viewport)
{
  GLFWViewport* p_viewport_cast = dynamic_cast<GLFWViewport*>(p_viewport);
  glfwSetWindowUserPointer(p_viewport_cast->getWindowPointer(), this);
  auto on_click_callback = [](GLFWwindow* w, int a, int b, int c)
  {
      static_cast<GLFWInput*>(glfwGetWindowUserPointer(w))->on_click_callback(w, a, b, c);
  };
  glfwSetMouseButtonCallback(p_viewport_cast->getWindowPointer(), on_click_callback);
  // auto MouseMoveCallback = [](GLFWwindow* w, int a, int b, int c)
  // {
  //     static_cast<GLFWInput*>(glfwGetWindowUserPointer(w))->MouseMoveCallback(w, a, b, c);
  // };
  // glfwSetCursorPosCallback	(p_viewport_cast->getWindowPointer(), MouseMoveCallback);
  auto on_key_press_callback = [](GLFWwindow* w, int a, int b, int c, int d)
  {
      static_cast<GLFWInput*>(glfwGetWindowUserPointer(w))->on_key_press_callback(w, a, b, c, d);
  };
  glfwSetKeyCallback(p_viewport_cast->getWindowPointer(), on_key_press_callback);
  auto on_window_resize_callback = [](GLFWwindow* w, int a, int b)
  {
      static_cast<GLFWInput*>(glfwGetWindowUserPointer(w))->on_window_resize_callback(w, a, b);
  };
  glfwSetWindowSizeCallback(p_viewport_cast->getWindowPointer(), on_window_resize_callback);
}
