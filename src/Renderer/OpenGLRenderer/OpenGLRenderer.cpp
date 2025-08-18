#include "OpenGLRenderer.h"

#define _DEBUG

template <typename T>
OpenGLRenderer<T>::OpenGLRenderer(
  IRecursiveRenderer<T>* p_recursive_renderer,
  Configuration configuration
) : p_recursive_renderer(p_recursive_renderer), configuration(configuration) {


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
}

template <typename T>
void OpenGLRenderer<T>::render_to_screen() {

}

template <typename T>
void OpenGLRenderer<T>::clear_screen() {
  glClear(GL_COLOR_BUFFER_BIT);
}