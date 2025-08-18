#include <utility>
#include <iostream>

// #include "src/glad.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "src/Configuration/Configuration.h"
#include "src/Camera/BasicCamera/BasicCamera.h"
#include "src/DataStructure2D/BasicDataStructure2D/BasicDataStructure2D.h"
#include "src/LongDouble/BasicLongDouble/BasicLongDouble.h"
#include "src/RecursiveRenderer/BasicRecursiveRenderer/BasicRecursiveRenderer.h"
#include "src/Shapes/Rectangle.h"
#include "src/helpers/Position.h"
#include "src/Renderer/OpenGLRenderer/OpenGLRenderer.h"
#include "src/Viewport/GLFWViewport/GLFWViewport.h"
#include "src/Input/GLFWInput/GLFWInput.h"

#define LongDouble BasicLongDouble

int main(int argc, char const *argv[])
{
  Configuration configuration(argc, argv);

  BasicDataStructure2D<LongDouble>* p_data_structure = new BasicDataStructure2D<LongDouble>;

  BasicCamera<LongDouble>* p_camera = new BasicCamera<LongDouble>(
    std::make_pair(
      Position<LongDouble>{LongDouble(0), LongDouble(0)},
      Position<LongDouble>{LongDouble(1), LongDouble(1)}
    )
  );

  BasicRecursiveRenderer<LongDouble>* p_recursiveRenderer = new BasicRecursiveRenderer<LongDouble>(p_data_structure, p_camera, configuration);

  p_recursiveRenderer->zoom_in(Position<LongDouble>{LongDouble(0.5), LongDouble(0.5)}, 0.5);
  p_recursiveRenderer->get_shapes_on_camera();
  p_recursiveRenderer->resize_camera(1, 1);

  GLFWViewport* p_viewport = new GLFWViewport();

  GLFWInput<LongDouble>* p_input = new GLFWInput<LongDouble>(GLFW_KEY_F, GLFW_KEY_ESCAPE, GLFW_KEY_X, GLFW_KEY_Z, GLFW_KEY_BACKSPACE, GLFW_KEY_CAPS_LOCK);
  p_input->subscribe_viewport_to_callbacks(p_viewport);
  p_viewport->subscribe_to_window_reconstruction(p_input);

  OpenGLRenderer<LongDouble>* renderer = new OpenGLRenderer<LongDouble>(p_recursiveRenderer, configuration);

  return 0;
}
