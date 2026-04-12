#include <utility>
#include <iostream>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#ifndef NO_OMP
#include <omp.h>
#endif
#include <Configuration/Configuration.h>
#include <Camera/BasicCamera/BasicCamera.h>
#include <DataStructure2D/BasicDataStructure2D/BasicDataStructure2D.h>
#include <LongDouble/BasicLongDouble/BasicLongDouble.h>
#include <RecursiveRenderer/BasicRecursiveRenderer/BasicRecursiveRenderer.h>
#include <Shapes/Rectangle.h>
#include <helpers/Position.h>
#include <Renderer/OpenGLRenderer/OpenGLRenderer.h>
#include <Viewport/GLFWViewport/GLFWViewport.h>
#include <Input/GLFWInput/GLFWInput.h>
#include <FractalDataStructure/FractalDataStructure.h>
#include <OnScreenButton/BasicSquareOnScreenButton/PredefinedButtons.h>
#include <LongDouble/LongDoubleBitset/LongDoubleBitset.h>
#include <DataStructure2D/QuadTree2D/QuadTree2D.h>
#include <DataStructure2D/RTree2D/RTree2D.h>
#include <LongDouble/LongDoubleUInt64/LongDoubleUInt64.h>
#include <Logger/Logger.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

// #define LongDouble _LongDoubleBitset
// #define LongDouble BasicLongDouble
// #define LongDouble LongDoubleUInt64<8>

template <typename T>
void loop(GLFWInput<T>* p_input, OpenGLRenderer<T>* p_renderer, GLFWViewport* p_viewport) {
  // glfwWaitEvents();

  p_input->send_recurring_events();
  
  auto window = p_viewport->getWindowPointer();
  double mouseX, mouseY;
  glfwGetCursorPos(window, &mouseX, &mouseY);
  int windowW, windowH;
  glfwGetWindowSize(window, &windowW, &windowH);
  Position<double> mouse_position = {mouseX / windowW, (- mouseY / windowH + 1.0)};
  p_input->notify_zoom(mouse_position, 1 / 0.985);

  // p_input->notify_zoom({0.5, 0.5}, 1 / 0.985);

  p_renderer->clear_screen();
  p_renderer->render_to_screen();

#ifndef __EMSCRIPTEN__
  glfwSwapBuffers(p_viewport->getWindowPointer());
  
  if(p_viewport->get_fullscreen_should_be_toggled()) {
    p_viewport->toggle_fullscreen();
  }
#endif
  
  glfwPollEvents();
}

template <typename T>
void emscripten_loop(void* arg) {
  void** args = (void**)arg;
  loop(
    static_cast<GLFWInput<T>*>(args[0]),
    static_cast<OpenGLRenderer<T>*>(args[1]),
    static_cast<GLFWViewport*>(args[2])
  );
}

template <typename T>
void initialize_loop(Configuration configuration) {
  // BasicDataStructure2D<T>* p_data_structure = new BasicDataStructure2D<T>;
  // QuadTree2D<T>* p_data_structure = new QuadTree2D<T>({-2, -2}, {2, 2}, configuration);
  // Logger::initialize("quadtree");
  // QuadTree2D<T>* p_data_structure = new QuadTree2D<T>({__DBL_MIN__, __DBL_MIN__}, {__DBL_MAX__, __DBL_MAX__}, configuration);
  RTree2D<T>* p_data_structure = new RTree2D<T>(configuration);
  Logger::initialize("r-tree");

  BasicCamera<T>* p_camera = new BasicCamera<T>(
    std::make_pair(
      Position<T>{T(-1), T(-1)},
      Position<T>{T(1), T(1)}
    )
  );

  FractalDataStructure<T>* p_fractal_data_structure = new FractalDataStructure<T>(p_camera, configuration);

  BasicRecursiveRenderer<T>* p_recursive_renderer = new BasicRecursiveRenderer<T>(p_data_structure, p_camera, p_fractal_data_structure, configuration);

  GLFWViewport* p_viewport = new GLFWViewport("Fractal Drawer 2.0", false);
  
  std::list<IOnScreenButton*> on_screen_buttons;
  IOnScreenButton* button_zoom_in = get_predefined_basic_on_screen_button_zoom_in();
  on_screen_buttons.push_back(button_zoom_in);
  IOnScreenButton* button_zoom_out = get_predefined_basic_on_screen_button_zoom_out();
  on_screen_buttons.push_back(button_zoom_out);

  OpenGLRenderer<T>* p_renderer = new OpenGLRenderer<T>(p_recursive_renderer, p_camera, configuration, on_screen_buttons);

  GLFWInput<T>* p_input = new GLFWInput<T>(p_viewport, button_zoom_in, button_zoom_out, GLFW_KEY_F, GLFW_KEY_ESCAPE, GLFW_KEY_X, GLFW_KEY_Z, GLFW_KEY_BACKSPACE, GLFW_KEY_CAPS_LOCK, GLFW_KEY_ENTER);
  
  p_input->process_window_reconstruction(p_viewport);
  p_input->subscribe_to_toggle_fullscreen(p_viewport);
  p_input->subscribe_to_zoom(p_camera);
  p_input->subscribe_to_zoom(p_fractal_data_structure); // after subscribing camers to zoom
  p_input->subscribe_to_zoom(p_recursive_renderer); // after subscribing camera and shape_ds to zoom
  p_input->subscribe_to_zoom_reset(p_camera);
  p_input->subscribe_to_window_resize(p_camera);
  p_input->subscribe_to_window_resize(p_renderer);
  p_viewport->subscribe_to_window_reconstruction(p_input);
  p_viewport->subscribe_to_window_reconstruction(p_renderer);
  p_input->subscribe_to_primary_button_down(p_fractal_data_structure);
  p_input->subscribe_to_secondary_button_down(p_fractal_data_structure);
  p_input->subscribe_to_pointer_move(p_fractal_data_structure);
  p_input->subscribe_to_confirm(p_fractal_data_structure);
  p_input->subscribe_to_clear(p_recursive_renderer);

  p_fractal_data_structure->process_primary_click({0, 0 - 0.366});
  p_fractal_data_structure->process_primary_click({1, 0 - 0.366});
  p_fractal_data_structure->process_secondary_click({0, 0 - 0.366});
  p_fractal_data_structure->process_secondary_click({0.5, 0 - 0.366});
  p_fractal_data_structure->process_secondary_click({1, 0 - 0.366}); // remove to simplify
  p_fractal_data_structure->process_secondary_click({0.75, 0.433 - 0.366}); // remove to simplify
  p_fractal_data_structure->process_secondary_click({0.5, 0.866 - 0.366});
  p_fractal_data_structure->process_secondary_click({0.25, 0.433 - 0.366});
  p_fractal_data_structure->process_confirm();


  // p_fractal_data_structure->process_primary_click({0.5, 0});
  // p_fractal_data_structure->process_primary_click({0.5, 0.25});
  // p_fractal_data_structure->process_secondary_click({0.5, 0.25});
  // p_fractal_data_structure->process_secondary_click({0.5, 0.25 + 0.125});
  // p_fractal_data_structure->process_secondary_click({0.5, 0.25});
  // p_fractal_data_structure->process_secondary_click({0.5 + 0.15, 0.25 + 0.05});
  // p_fractal_data_structure->process_confirm();

  std::pair<int, int> size = p_viewport->get_size();
  std::cout << size.first << " " << size.second << " - size\n";
  p_camera->process_window_resize(size.first, size.second);
  p_renderer->process_window_resize(size.first, size.second);
  
#ifdef __EMSCRIPTEN__
  void** args_arr = new void*[3];
  args_arr[0] = (void*)p_input;
  args_arr[1] = (void*)p_renderer;
  args_arr[2] = (void*)p_viewport;
  emscripten_set_main_loop_arg(&emscripten_loop<T>, args_arr, -1, 1);
#else
  while (!p_viewport->window_should_close()) {   
    loop<T>(p_input, p_renderer, p_viewport);
  }
#endif
}

int main(int argc, char const *argv[])
{
  Configuration configuration;
  // if (argc > 1)
  // {
  //   configuration = Configuration(argc, argv);
  // } else {
    configuration = Configuration("res/config.txt");
  // }
  
#ifndef NO_OMP
  omp_set_num_threads(configuration.threads);
#endif
  
  if (configuration.model_type == FAST)
  {
    initialize_loop<BasicLongDouble>(configuration);
  } else if (configuration.model_type == DEEP) {
    initialize_loop<LongDoubleUInt64<8>>(configuration);
  }

  return 0;
}
