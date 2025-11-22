//
// display: pheromone_matrix, best_permutation, job_tardiness, job size, right answer
//

#include <utility>
#include <iostream>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#ifndef NO_OMP
#include <omp.h>
#endif
#include <Configuration/Configuration.h>
#include <Camera/BasicCamera/BasicCamera.h>
#include <LongDouble/BasicLongDouble/BasicLongDouble.h>
// #include <Shapes/Rectangle.h>
#include <helpers/Position.h>
#include <Renderer/OpenGLRenderer/OpenGLRenderer.h>
#include <Viewport/GLFWViewport/GLFWViewport.h>
#include <Input/GLFWInput/GLFWInput.h>
#include <OnScreenButton/BasicSquareOnScreenButton/PredefinedButtons.h>
#include <LongDouble/LongDoubleBitset/LongDoubleBitset.h>
// #include <DataStructure2D/QuadTree2D/QuadTree2D.h>
#include <LongDouble/LongDoubleUInt64/LongDoubleUInt64.h>
#include <ACODataProvider/ACODataProvider.h>
#include <ACOOpenGLRenderer/ACOOpenGLRenderer.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

template <typename T>
void loop(GLFWInput<T>* p_input, ACOOpenGLRenderer<T>* p_renderer, GLFWViewport* p_viewport) {
  // glfwWaitEvents();

  p_input->send_recurring_events();
  
  p_renderer->clear_screen();
  p_renderer->render_aco_to_screen();

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
    static_cast<ACOOpenGLRenderer<T>*>(args[1]),
    static_cast<GLFWViewport*>(args[2])
  );
}

template <typename T>
void initialize_loop(Configuration configuration) {
  // BasicDataStructure2D<T>* p_data_structure = new BasicDataStructure2D<T>;
  // QuadTree2D<T>* p_data_structure = new QuadTree2D<T>({-2, -2}, {2, 2}, configuration);

  BasicCamera<T>* p_camera = new BasicCamera<T>(
    std::make_pair(
      Position<T>{T(-1), T(-1)},
      Position<T>{T(1), T(1)}
    )
  );

  ACODataProvider<T>* p_data_provider = new ACODataProvider<T>(10, 10, 100, 50, 20, 100000, 0.95);
  p_data_provider->print_scores();
  
  GLFWViewport* p_viewport = new GLFWViewport("Fractal Drawer 2.0", false);
  
  std::list<IOnScreenButton*> on_screen_buttons;
  IOnScreenButton* button_zoom_in = get_predefined_basic_on_screen_button_zoom_in();
  on_screen_buttons.push_back(button_zoom_in);
  IOnScreenButton* button_zoom_out = get_predefined_basic_on_screen_button_zoom_out();
  on_screen_buttons.push_back(button_zoom_out);

  ACOOpenGLRenderer<T>* p_renderer = new ACOOpenGLRenderer<T>(p_data_provider, p_camera, configuration, on_screen_buttons);

  GLFWInput<T>* p_input = new GLFWInput<T>(p_viewport, button_zoom_in, button_zoom_out, GLFW_KEY_F, GLFW_KEY_ESCAPE, GLFW_KEY_X, GLFW_KEY_Z, GLFW_KEY_BACKSPACE, GLFW_KEY_CAPS_LOCK, GLFW_KEY_ENTER);
  
  p_input->process_window_reconstruction(p_viewport);
  p_input->subscribe_to_toggle_fullscreen(p_viewport);
  p_input->subscribe_to_zoom(p_camera);
  p_input->subscribe_to_zoom_reset(p_camera);
  p_input->subscribe_to_window_resize(p_camera);
  p_input->subscribe_to_window_resize(p_renderer);
  p_viewport->subscribe_to_window_reconstruction(p_input);
  p_viewport->subscribe_to_window_reconstruction(p_renderer);
  p_input->subscribe_to_confirm(p_data_provider);
  p_input->subscribe_to_clear(p_data_provider);

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
  configuration = Configuration("res/config.txt");
  
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
