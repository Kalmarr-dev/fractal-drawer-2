#include "GLFWViewport.h"

#include <iostream>
#include "../../../include/glad/glad.h"

GLFWViewport::GLFWViewport() {
  glfwInit();

  // const char* description;
  // int code = glfwGetError(&description);
  
  // if (description)
  //   std::cout << code << '\n';
  //   display_error_message(code, description);
  
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
  this->p_window = create_windowed_window(1280, 720);
  if (!this->p_window) {
    glfwTerminate();
    throw "No GLFW window context, terminating";
  }

  glfwMakeContextCurrent(this->p_window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    throw "Failed to initialize GLAD";
  }

  glfwSwapInterval(1);

  #ifdef _DEBUG
    if(glDebugMessageCallback){
      std::cout << "Register OpenGL debug callback " << std::endl;
      glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
      glDebugMessageCallback(openglErrorHandlingCallback, nullptr);
      GLuint unusedIds = 0;
      glDebugMessageControl(GL_DONT_CARE,
          GL_DONT_CARE,
          GL_DONT_CARE,
          0,
          &unusedIds,
          true);
    }
    else
      std::cout << "glDebugMessageCallback not available" << std::endl;
  #endif

  glEnable(GL_POINT_SMOOTH);
}

GLFWViewport::~GLFWViewport() {
  glfwTerminate();
}

std::pair<int, int> GLFWViewport::get_size() {
  int windowW, windowH;
  glfwGetWindowSize(p_window, &windowW, &windowH);
  return std::make_pair(windowW, windowH);
}

GLFWwindow* GLFWViewport::create_windowed_window(int width, int height) {
  glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
  GLFWwindow* window = glfwCreateWindow(width, height, "Fractal Drawer", NULL, this->p_window);
  // glfwMakeContextCurrent(window);
  // glfwSetMouseButtonCallback(window, Input::MouseButtonCallback);
  // glfwSetKeyCallback(window, Input::KeyCallback);
  // glfwSetWindowSizeCallback(window, WindowResizeCallback);
  return window;
}
GLFWwindow* GLFWViewport::create_fullscreen_window(int width, int height) {
  glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
  GLFWwindow* window = glfwCreateWindow(width, height, "Fractal Drawer", glfwGetPrimaryMonitor(), this->p_window);
  // glfwMakeContextCurrent(window);
  // glfwSetMouseButtonCallback(window, Input::MouseButtonCallback);
  // glfwSetKeyCallback(window, Input::KeyCallback);
  // glfwSetWindowSizeCallback(window, WindowResizeCallback);
  return window;
}

void GLFWViewport::subscribe_to_window_reconstruction(IInputObserver* observer) {
  input_observers.remove(observer);
  input_observers.push_back(observer);
}

void GLFWViewport::unsubscribe_from_window_reconstruction(IInputObserver* observer) {
  input_observers.remove(observer);
}

void GLFWViewport::notify_window_reconstruction() {
  for (auto &&observer : input_observers)
  {
    observer->subscribe_viewport_to_callbacks(this);
  }
}

GLFWwindow* GLFWViewport::getWindowPointer() {
  return p_window;
}
