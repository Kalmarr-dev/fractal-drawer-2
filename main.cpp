#include <utility>
#include <iostream>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <omp.h>

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
#include "src/ShapeDataStructure/FractalDataStructure/FractalDataStructure.h"
#include "src/OnScreenButton/BasicSquareOnScreenButton/PredefinedButtons.h"
#include "src/LongDouble/LongDoubleBitset/LongDoubleBitset.h"
#include "src/DataStructure2D/QuadTree2D/QuadTree2D.h"
#include "src/LongDouble/LongDoubleUInt64/LongDoubleUInt64.h"

// #define LongDouble _LongDoubleBitset
// #define LongDouble BasicLongDouble
// #define LongDouble LongDoubleUInt64<8>

template <typename T>
void initialize_loop(Configuration configuration) {
  // BasicDataStructure2D<T>* p_data_structure = new BasicDataStructure2D<T>;
  QuadTree2D<T>* p_data_structure = new QuadTree2D<T>({-2, -2}, {2, 2}, configuration);

  BasicCamera<T>* p_camera = new BasicCamera<T>(
    std::make_pair(
      Position<T>{T(-1), T(-1)},
      Position<T>{T(1), T(1)}
    )
  );

  FractalDataStructure<T>* p_fractal_data_structure = new FractalDataStructure<T>(p_camera, configuration);

  BasicRecursiveRenderer<T>* p_recursive_renderer = new BasicRecursiveRenderer<T>(p_data_structure, p_camera, p_fractal_data_structure, configuration);

  GLFWViewport* p_viewport = new GLFWViewport(true);
  
  std::list<IOnScreenButton*> on_screen_buttons;
  IOnScreenButton* button_zoom_in = get_predefined_basic_on_screen_button_zoom_in();
  on_screen_buttons.push_back(button_zoom_in);
  IOnScreenButton* button_zoom_out = get_predefined_basic_on_screen_button_zoom_out();
  on_screen_buttons.push_back(button_zoom_out);

  OpenGLRenderer<T>* p_renderer = new OpenGLRenderer<T>(p_recursive_renderer, p_camera, configuration, on_screen_buttons);

  GLFWInput<T>* p_input = new GLFWInput<T>(p_viewport, button_zoom_in, button_zoom_out, GLFW_KEY_F, GLFW_KEY_ESCAPE, GLFW_KEY_X, GLFW_KEY_Z, GLFW_KEY_BACKSPACE, GLFW_KEY_CAPS_LOCK, GLFW_KEY_ENTER);
  
  p_input->subscribe_viewport_to_callbacks(p_viewport);
  p_input->subscribe_to_toggle_fullscreen(p_viewport);
  p_input->subscribe_to_zoom(p_camera);
  p_input->subscribe_to_zoom(p_fractal_data_structure); // after subscribing camers to zoom
  p_input->subscribe_to_zoom(p_recursive_renderer); // after subscribing camera and shape_ds to zoom
  p_input->subscribe_to_zoom_reset(p_camera);
  p_input->subscribe_to_window_resize(p_camera);
  p_input->subscribe_to_window_resize(p_renderer);
  p_viewport->subscribe_to_window_reconstruction(p_input);
  p_input->subscribe_to_primary_button_down(p_fractal_data_structure);
  p_input->subscribe_to_secondary_button_down(p_fractal_data_structure);
  p_input->subscribe_to_pointer_move(p_fractal_data_structure);
  p_input->subscribe_to_confirm(p_fractal_data_structure);


  while (!p_viewport->window_should_close()) {   
    // glfwWaitEvents();

    p_input->send_recurring_events();
    
    p_renderer->clear_screen();
    p_renderer->render_to_screen();

 
    glfwSwapBuffers(p_viewport->getWindowPointer());
    
    if(p_viewport->get_fullscreen_should_be_toggled()) {
      p_viewport->toggle_fullscreen();
    }

    glfwPollEvents();
  }
}

int main(int argc, char const *argv[])
{
  Configuration configuration;
  if (argc > 1)
  {
    configuration = Configuration(argc, argv);
  } else {
    configuration = Configuration("res/config.txt");
  }
  
  omp_set_num_threads(configuration.threads);
  
  if (configuration.model_type == FAST)
  {
    initialize_loop<BasicLongDouble>(configuration);
  } else if (configuration.model_type == DEEP) {
    initialize_loop<LongDoubleUInt64<8>>(configuration);
  }

  return 0;
}
