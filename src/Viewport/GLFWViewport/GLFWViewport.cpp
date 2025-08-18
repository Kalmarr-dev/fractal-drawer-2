#include "GLFWViewport.h"

#include <iostream>
#include "../../../include/glad/glad.h"

GLFWVieport::GLFWVieport() {
  glfwInit();

  // const char* description;
  // int code = glfwGetError(&description);
  
  // if (description)
  //   std::cout << code << '\n';
  //   display_error_message(code, description);
  
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
  this->p_window = glfwCreateWindow(1280, 720, "Fractal Drawer", NULL, NULL);
  if (!this->p_window) {
    std::cerr << "No GLFW window context, terminating" << '\n';
    glfwTerminate();
  }
}

GLFWVieport::~GLFWVieport() {
  glfwTerminate();
}
