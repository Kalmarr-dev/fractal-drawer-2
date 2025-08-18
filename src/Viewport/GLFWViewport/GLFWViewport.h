#pragma once

#include <list>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "../IViewport.h"

class GLFWViewport : public IViewport
{
private:
  GLFWwindow* p_window;
  std::list<IInputObserver*> input_observers;

  GLFWwindow* create_windowed_window(int width, int height);
  GLFWwindow* create_fullscreen_window(int width, int height);
public:
  GLFWViewport();
  ~GLFWViewport() override;

  std::pair<int, int> get_size() override;
  void subscribe_to_window_reconstruction(IInputObserver* observer) override;
  void unsubscribe_from_window_reconstruction(IInputObserver* observer) override;
  void notify_window_reconstruction() override;

  GLFWwindow* getWindowPointer();
};
