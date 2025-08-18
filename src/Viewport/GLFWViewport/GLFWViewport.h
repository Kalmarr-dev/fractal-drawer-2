#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "../IViewport.h"

class GLFWVieport : public IViewport
{
private:
  GLFWwindow* p_window;
public:
  GLFWVieport();
  ~GLFWVieport() override;
};
