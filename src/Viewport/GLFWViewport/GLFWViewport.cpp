#include "GLFWViewport.h"

#include <iostream>
#ifndef __EMSCRIPTEN__
#include "../../../include/glad/glad.h"
#else
#include "../../../include/glad_es/glad.h"
#endif

void error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error (%d): %s\n", error, description);
}

GLFWViewport::GLFWViewport(std::string window_name, bool fullscreen) : window_name(window_name) {
  glfwInit();

  glfwSetErrorCallback(error_callback);
  // const char* description;
  // int code = glfwGetError(&description);
  
  // if (description)
  //   std::cout << code << '\n';
  //   display_error_message(code, description);
  
  #ifdef _DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
  #endif
  this->is_fullscreen = !fullscreen;
  this->p_window = nullptr;
  this->toggle_fullscreen();
  if (!this->p_window) {
    glfwTerminate();
    throw std::runtime_error("No GLFW window context, terminating");
  }

  glfwMakeContextCurrent(this->p_window);

#ifndef __EMSCRIPTEN__
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
  }

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    throw std::runtime_error("Failed to initialize GLAD");
  }
#else
  if (!gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress))
  {
    throw std::runtime_error("Failed to initialize GLAD");
  }
#endif

  glfwSwapInterval(1);

#ifndef __EMSCRIPTEN__
  glEnable(GL_POINT_SMOOTH);
#endif
}

GLFWViewport::~GLFWViewport() {
  glfwTerminate();
}

std::pair<int, int> GLFWViewport::get_size() {
  int bufferW, bufferH;
  glfwGetFramebufferSize(p_window, &bufferW, &bufferH);
  // const char* description;
  // int code = glfwGetError(&description);
  // if (description) {
  //   std::cout << code << '\n';
  //   std::cout << description << '\n';
  // }
  return std::make_pair(bufferW, bufferH);
}

GLFWwindow* GLFWViewport::create_windowed_window(int width, int height) {
  glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
  GLFWwindow* window = glfwCreateWindow(width, height, this->window_name.c_str(), NULL, this->p_window);
  // const char* description;
  // int code = glfwGetError(&description);
  // if (description) {
  //   std::cout << code << '\n';
  //   std::cout << description << '\n';
  // }
  glfwMakeContextCurrent(window);
  return window;
}
GLFWwindow* GLFWViewport::create_fullscreen_window(int width, int height) {
  glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
  GLFWwindow* window = glfwCreateWindow(width, height, this->window_name.c_str(), glfwGetPrimaryMonitor(), this->p_window);
  glfwMakeContextCurrent(window);
  return window;
}

void GLFWViewport::subscribe_to_window_reconstruction(IWindowReconstructionObserver* observer) {
  input_observers.remove(observer);
  input_observers.push_back(observer);
}

void GLFWViewport::unsubscribe_from_window_reconstruction(IWindowReconstructionObserver* observer) {
  input_observers.remove(observer);
}

void GLFWViewport::notify_window_reconstruction() {
  for (auto &&observer : input_observers)
  {
    observer->process_window_reconstruction(this);
  }
}

bool GLFWViewport::window_should_close() {
  return glfwWindowShouldClose(p_window);
}

void GLFWViewport::toggle_fullscreen() {
  if (this->is_fullscreen) {
    auto previous_window = this->p_window;
    this->p_window = create_windowed_window(1280, 720);
    glfwDestroyWindow(previous_window);
    this->is_fullscreen = false;
  } else {
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    auto previous_window = this->p_window;
    p_window = create_fullscreen_window(mode->width, mode->height);
    glfwDestroyWindow(previous_window);
    this->is_fullscreen = true;
  }
  if (!p_window) {
    std::cout << "Failed to create window\n" << std::endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  fullscreen_should_be_toggled = false;
  notify_window_reconstruction();
}

void GLFWViewport::set_fullscreen_should_be_toggled() {
  fullscreen_should_be_toggled = true;
}

bool GLFWViewport::get_fullscreen_should_be_toggled() {
  return fullscreen_should_be_toggled;
}

GLFWwindow* GLFWViewport::getWindowPointer() {
  return p_window;
}
