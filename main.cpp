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
// #include "src/LongDouble/LongDoubleVector/LongDoubleVector.h"
#include "src/LongDouble/LongDoubleBitset/LongDoubleBitset.h"

#define LongDouble BasicLongDouble
#define LongDoubleBitset LongDoubleBitset<1024>

int main(int argc, char const *argv[])
{
  LongDoubleBitset* zero_ldb = new LongDoubleBitset(0);
  std::cout << zero_ldb->get_double(zero_ldb, 0) << '\n';
  LongDoubleBitset ldb(-100000.000001);
  std::cout << ldb.get_double(zero_ldb, 0) << '\n';
  LongDoubleBitset ldb1(-0.0000000000001);
  std::cout << ldb1.get_double(zero_ldb, 0) << '\n';
  LongDoubleBitset ldb2(-0.00000000000010001);
  std::cout << ldb2.get_double(zero_ldb, 0) << '\n';
  
  // LongDoubleBitset ldb1_zero = ldb1 + *zero_ldb;
  // LongDoubleBitset ldb1_1 = ldb1 + ldb1;
  // std::cout << ldb1_zero.get_double(zero_ldb, 0) << '\n';
  // std::cout << ldb1_1.get_double(zero_ldb, 0) << '\n';
  // std::cout << (LongDoubleBitset(100) + LongDoubleBitset(10)).get_double(zero_ldb, 0) << '\n';
  // std::cout << (LongDoubleBitset(100) < LongDoubleBitset(10)) << " false" << '\n';
  // std::cout << (LongDoubleBitset(0.01) < LongDoubleBitset(0.1)) << " true" << '\n';
  // std::cout << (LongDoubleBitset(0.0000011) < LongDoubleBitset(0.000001)) << " false" << '\n';
  // std::cout << (LongDoubleBitset(0.000001) < LongDoubleBitset(0.0000011)) << " true" << '\n';
  // std::cout << (LongDoubleBitset(-100) < LongDoubleBitset(-10)) << " true" << '\n';
  // std::cout << (LongDoubleBitset(100) - LongDoubleBitset(10)).get_double(zero_ldb, 0) << '\n';
  std::cout << (LongDoubleBitset(101) * LongDoubleBitset(10)).get_double(zero_ldb, 0) << '\n';
  std::cout << (LongDoubleBitset(7) * LongDoubleBitset(0.15)).get_double(zero_ldb, 0) << '\n';
  std::cout << (LongDoubleBitset(0.01) * LongDoubleBitset(0.101)).get_double(zero_ldb, 0) << '\n';


  Configuration configuration;
  if (argc > 1)
  {
    configuration = Configuration(argc, argv);
  } else {
    configuration = Configuration("res/config.txt");
  }
  
  omp_set_num_threads(configuration.threads);

  BasicDataStructure2D<LongDouble>* p_data_structure = new BasicDataStructure2D<LongDouble>;

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
  // p_input->subscribe_to_primary_button_down(p_recursive_renderer); // after subscribing DS
  // p_input->subscribe_to_secondary_button_down(p_recursive_renderer); // after subscribing DS
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
