#pragma once

#include <list>
#include <string>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "../IViewport.h"
#include "../IViewportObserver.h"

class GLFWViewport : public IViewport, public IViewportObserver
{
private:
  GLFWwindow* p_window;
  std::list<IWindowReconstructionObserver*> input_observers;
  std::string window_name;
  bool is_fullscreen;
  bool fullscreen_should_be_toggled = false;

  GLFWwindow* create_windowed_window(int width, int height);
  GLFWwindow* create_fullscreen_window(int width, int height);
public:
  GLFWViewport(std::string window_name, bool is_fullscreen = false);
  ~GLFWViewport() override;

  std::pair<int, int> get_size() override;
  void subscribe_to_window_reconstruction(IWindowReconstructionObserver* observer) override;
  void unsubscribe_from_window_reconstruction(IWindowReconstructionObserver* observer) override;
  void notify_window_reconstruction() override;
  bool window_should_close() override;
  void toggle_fullscreen() override;
  void set_fullscreen_should_be_toggled() override;

  // TODO delete void get_fullscreen_should_be_toggled
  bool get_fullscreen_should_be_toggled();
  GLFWwindow* getWindowPointer();
};
