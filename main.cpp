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
#define LongDouble BasicLongDouble
// #define LongDouble LongDoubleUInt64<8>

int main(int argc, char const *argv[])
{
  LongDoubleUInt64<8> offset_zero(0);
  // std::cout << LongDoubleUInt64<8>(3).get_double(&offset_zero, 0) << '\n';
  // std::cout << LongDoubleUInt64<8>(0.3).get_double(&offset_zero, 0) << '\n';
  // std::cout << LongDoubleUInt64<8>(-0.3).get_double(&offset_zero, 0) << '\n';
  // std::cout << (LongDoubleUInt64<8>(-0.3) < LongDoubleUInt64<8>(-0.4)) << '\n';
  // std::cout << (LongDoubleUInt64<8>(0.3) < LongDoubleUInt64<8>(0.4)) << '\n';
  // std::cout << (LongDoubleUInt64<8>(0.3) + LongDoubleUInt64<8>(0.4)).get_double(&offset_zero, 0) << '\n';
  // std::cout << (LongDoubleUInt64<8>(0.3) + LongDoubleUInt64<8>(-0.4)).get_double(&offset_zero, 0) << '\n';
  // std::cout << (LongDoubleUInt64<8>(1.0 / (uint64_t(1) << 32) / (uint64_t(1) << 32)) + LongDoubleUInt64<8>(1)).get_double(&offset_zero, 0) << '\n';
  // std::cout << (LongDoubleUInt64<8>(0.4) - LongDoubleUInt64<8>(0.3)).get_double(&offset_zero, 0) << '\n';
  // std::cout << (LongDoubleUInt64<8>(2) * LongDoubleUInt64<8>(0.5)).get_double(&offset_zero, 0) << '\n';
  // std::cout << (LongDoubleUInt64<8>(0.4) * LongDoubleUInt64<8>(0.3)).get_double(&offset_zero, 0) << '\n';
  // std::cout << (LongDoubleUInt64<8>(4321) * LongDoubleUInt64<8>(0.321)).get_double(&offset_zero, 0) << '\n';
  // std::cout << (LongDoubleUInt64<8>(0.9) * LongDoubleUInt64<8>(0.9)).get_double(&offset_zero, 0) << '\n';
  // std::cout << (LongDoubleUInt64<8>(2.0) / LongDoubleUInt64<8>(0.5)).get_double(&offset_zero, 0) << '\n';
  // std::cout << (LongDoubleUInt64<8>(0.9) / LongDoubleUInt64<8>(0.9)).get_double(&offset_zero, 0) << '\n';
  // std::cout << (LongDoubleUInt64<8>(1010) / LongDoubleUInt64<8>(0.101)).get_double(&offset_zero, 0) << '\n';
  // std::cout << (LongDoubleUInt64<8>(1) / LongDoubleUInt64<8>(3)).get_double(&offset_zero, 0) << '\n';

  Configuration configuration;
  if (argc > 1)
  {
    configuration = Configuration(argc, argv);
  } else {
    configuration = Configuration("res/config.txt");
  }
  
  omp_set_num_threads(configuration.threads);

  // BasicDataStructure2D<LongDouble>* p_data_structure = new BasicDataStructure2D<LongDouble>;
  QuadTree2D<LongDouble>* p_data_structure = new QuadTree2D<LongDouble>({-2, -2}, {2, 2}, configuration);

  BasicCamera<LongDouble>* p_camera = new BasicCamera<LongDouble>(
    std::make_pair(
      Position<LongDouble>{LongDouble(-1), LongDouble(-1)},
      Position<LongDouble>{LongDouble(1), LongDouble(1)}
    )
  );

  FractalDataStructure<LongDouble>* p_fractal_data_structure = new FractalDataStructure<LongDouble>(p_camera, configuration);

  BasicRecursiveRenderer<LongDouble>* p_recursive_renderer = new BasicRecursiveRenderer<LongDouble>(p_data_structure, p_camera, p_fractal_data_structure, configuration);

  // p_camera->resize(16.0 / 9.0);

  GLFWViewport* p_viewport = new GLFWViewport(true);
  
  std::list<IOnScreenButton*> on_screen_buttons;
  IOnScreenButton* button_zoom_in = get_predefined_basic_on_screen_button_zoom_in();
  on_screen_buttons.push_back(button_zoom_in);
  IOnScreenButton* button_zoom_out = get_predefined_basic_on_screen_button_zoom_out();
  on_screen_buttons.push_back(button_zoom_out);

  OpenGLRenderer<LongDouble>* p_renderer = new OpenGLRenderer<LongDouble>(p_recursive_renderer, p_camera, configuration, on_screen_buttons);

  GLFWInput<LongDouble>* p_input = new GLFWInput<LongDouble>(p_viewport, button_zoom_in, button_zoom_out, GLFW_KEY_F, GLFW_KEY_ESCAPE, GLFW_KEY_X, GLFW_KEY_Z, GLFW_KEY_BACKSPACE, GLFW_KEY_CAPS_LOCK, GLFW_KEY_ENTER);
  
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
  

  return 0;
}
