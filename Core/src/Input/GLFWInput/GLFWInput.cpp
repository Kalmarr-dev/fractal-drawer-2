#include "GLFWInput.h"

#include <utility>
#include <math.h>
#include "../../Viewport/GLFWViewport/GLFWViewport.h"

template<typename T>
GLFWInput<T>::GLFWInput(IViewport* p_viewport, GLFWKey fullscreen_key, GLFWKey zoom_reset_key, 
  GLFWKey zoom_in_key, GLFWKey zoom_out_key, GLFWKey clear_key, GLFWKey lock_zoom_key, GLFWKey confirm_key
) : p_viewport(p_viewport), fullscreen_key(fullscreen_key), zoom_reset_key(zoom_reset_key),
    zoom_in_key(zoom_in_key), zoom_out_key(zoom_out_key), clear_key(clear_key), lock_zoom_key(lock_zoom_key),
    confirm_key(confirm_key)
{
  this->button_zoom_in = nullptr;
  this->button_zoom_out = nullptr;
}

template<typename T>
GLFWInput<T>::GLFWInput(IViewport* p_viewport, IOnScreenButton* button_zoom_in, IOnScreenButton* button_zoom_out,
  GLFWKey fullscreen_key, GLFWKey zoom_reset_key, 
  GLFWKey zoom_in_key, GLFWKey zoom_out_key,
  GLFWKey clear_key, GLFWKey lock_zoom_key, GLFWKey confirm_key
) : p_viewport(p_viewport), button_zoom_in(button_zoom_in), button_zoom_out(button_zoom_out),
    fullscreen_key(fullscreen_key), zoom_reset_key(zoom_reset_key), zoom_in_key(zoom_in_key), zoom_out_key(zoom_out_key),
    clear_key(clear_key), lock_zoom_key(lock_zoom_key), confirm_key(confirm_key)
{}

template<typename T>
void GLFWInput<T>::on_click_callback(GLFWwindow* window, int button, int action, int mods) {
  double mouseX, mouseY;
  glfwGetCursorPos(window, &mouseX, &mouseY);
  int windowW, windowH;
  glfwGetWindowSize(window, &windowW, &windowH);
  Position<double> mouse_position = {mouseX / windowW, (- mouseY / windowH + 1.0)};

  if (action == GLFW_PRESS) {
    bool button_was_pressed = false;
    if (this->button_zoom_in != nullptr && this->button_zoom_in->pointer_is_inside(mouse_position, windowW, windowH))
    {
      this->button_zoom_in->set_clicked();
      button_was_pressed = true;
    }
    if (this->button_zoom_out != nullptr && this->button_zoom_out->pointer_is_inside(mouse_position, windowW, windowH))
    {
      this->button_zoom_out->set_clicked();
      button_was_pressed = true;
    }
    if (!button_was_pressed)
    {
      if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        this->notify_secondary_button_down(mouse_position);
      } else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        // TODO
        // throw "Not implemented";
      } else {
        this->notify_primary_button_down(mouse_position);
      }
    }
  }

  if (action == GLFW_RELEASE) {
    this->button_zoom_in->set_not_clicked();
    this->button_zoom_out->set_not_clicked();
    this->notify_pointer_up();
  }
}

template<typename T>
void GLFWInput<T>::on_pointer_move_callback(GLFWwindow* window, double x, double y) {
  int windowW, windowH;
  glfwGetWindowSize(window, &windowW, &windowH);
  this->notify_pointer_move(Position<double>{x / windowW, (- y / windowH + 1.0)});
}

template<typename T>
void GLFWInput<T>::on_key_press_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  double mouseX, mouseY;
  glfwGetCursorPos(window, &mouseX, &mouseY);
  int windowW, windowH;
  glfwGetWindowSize(window, &windowW, &windowH);
  if (key == confirm_key.code && action == GLFW_PRESS) {
    this->notify_confirm();
  }
  if (key == clear_key.code && action == GLFW_PRESS) {
    this->notify_clear();
  }
  if (key == zoom_reset_key.code && action == GLFW_PRESS) {
    this->notify_zoom_reset();
  }
  if (key == zoom_out_key.code && action != GLFW_RELEASE) {
    if (lock_zoom_key.amount_of_times_clicked && action == GLFW_PRESS)
    {
      this->zoom_out_key.amount_of_times_clicked++;
    }
    this->notify_zoom(Position<double>{mouseX / windowW, (- mouseY / windowH + 1.0)}, 0.995);
  }
  if (key == zoom_in_key.code && action != GLFW_RELEASE) {
    if (lock_zoom_key.amount_of_times_clicked && action == GLFW_PRESS)
    {
      this->zoom_in_key.amount_of_times_clicked++;
    }
    this->notify_zoom(Position<double>{mouseX / windowW, (- mouseY / windowH + 1.0)}, 1.0055555555);
  }
  if (key == lock_zoom_key.code && action == GLFW_PRESS) {
    lock_zoom_key.amount_of_times_clicked = (mods & GLFW_MOD_CAPS_LOCK) ? 1 : 0;
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
  if (width != 0 && height != 0)
  {
    this->notify_window_resize(width, height);
  }
}

template<typename T>
void GLFWInput<T>::process_window_reconstruction(IViewport* p_viewport)
{
  GLFWViewport* p_viewport_cast = dynamic_cast<GLFWViewport*>(p_viewport);
  glfwSetInputMode(p_viewport_cast->getWindowPointer(), GLFW_LOCK_KEY_MODS, GLFW_TRUE);
  glfwSetWindowUserPointer(p_viewport_cast->getWindowPointer(), this);
  auto on_click_callback = [](GLFWwindow* w, int a, int b, int c)
  {
      static_cast<GLFWInput*>(glfwGetWindowUserPointer(w))->on_click_callback(w, a, b, c);
  };
  glfwSetMouseButtonCallback(p_viewport_cast->getWindowPointer(), on_click_callback);
  auto on_pointer_move_callback = [](GLFWwindow* w, double x, double y)
  {
      static_cast<GLFWInput*>(glfwGetWindowUserPointer(w))->on_pointer_move_callback(w, x, y);
  };
  glfwSetCursorPosCallback(p_viewport_cast->getWindowPointer(), on_pointer_move_callback);
  auto on_key_press_callback = [](GLFWwindow* w, int a, int b, int c, int d)
  {
      static_cast<GLFWInput*>(glfwGetWindowUserPointer(w))->on_key_press_callback(w, a, b, c, d);
  };
  glfwSetKeyCallback(p_viewport_cast->getWindowPointer(), on_key_press_callback);
  auto on_window_resize_callback = [](GLFWwindow* w, int a, int b)
  {
      static_cast<GLFWInput*>(glfwGetWindowUserPointer(w))->on_window_resize_callback(w, a, b);
  };
  glfwSetFramebufferSizeCallback(p_viewport_cast->getWindowPointer(), on_window_resize_callback);
}

template<typename T>
void GLFWInput<T>::send_recurring_events() {
  GLFWViewport* p_viewport_cast = dynamic_cast<GLFWViewport*>(p_viewport);
  double mouseX, mouseY;
  glfwGetCursorPos(p_viewport_cast->getWindowPointer(), &mouseX, &mouseY);
  int windowW, windowH;
  glfwGetWindowSize(p_viewport_cast->getWindowPointer(), &windowW, &windowH);
  auto mouse_position = Position<double>{mouseX / windowW, (- mouseY / windowH + 1.0)};
  if (this->button_zoom_in != nullptr && this->button_zoom_in->is_clicked())
  {
    this->notify_zoom(mouse_position, 1 / 0.995);
  } else if (this->button_zoom_out != nullptr && this->button_zoom_out->is_clicked())
  {
    this->notify_zoom(mouse_position, 0.995);
  } else if (this->lock_zoom_key.amount_of_times_clicked)
  {
    this->notify_zoom
    (
      mouse_position,
      std::pow(0.9975, this->zoom_out_key.amount_of_times_clicked - this->zoom_in_key.amount_of_times_clicked)
    );
  } else {
    this->zoom_in_key.amount_of_times_clicked = 0;
    this->zoom_out_key.amount_of_times_clicked = 0;
  }
}
