#include "OpenGLRenderer.h"

#include <iostream>

#define _DEBUG

template <typename T>
OpenGLRenderer<T>::OpenGLRenderer(
  IRecursiveRenderer<T>* p_recursive_renderer,
  ICamera<T>* p_camera,
  Configuration configuration
) : p_recursive_renderer(p_recursive_renderer), p_camera(p_camera), configuration(configuration) {

  // this->vertex_array = new;
  // this->index_buffer.Unbind();

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
OpenGLRenderer<T>::~OpenGLRenderer() {
  this->vertex_array->Unbind();
  this->index_buffer->Unbind();
}

template <typename T>
void OpenGLRenderer<T>::render_to_screen() {
  // Shapes<T> shapes = p_recursive_renderer->get_shapes_on_camera();
  // for (auto &&shape : shapes)
  // {
    
  // }

  
  float positions[] = {0, 0, 0.5f, 0.5f};
  unsigned int indices[] = {0, 1};

  VertexBuffer vb(positions, 4 * sizeof(float));
  IndexBuffer ib(indices, (unsigned int)2);

  VertexBufferLayout layout;
  layout.Push<float>(2);

  VertexArray va;
  va.AddBuffer(vb, layout);

  Shader shader("res/shaders/basic.shader");
  shader.Bind();

  auto camera_corners = p_camera->get_camera_corners();
  T* offset = new T(0);
  shader.SetUniform4f
  ( 
    "u_camera", camera_corners.first.x.get_double(offset, 1),
    camera_corners.first.y.get_double(offset, 1),
    camera_corners.second.x.get_double(offset, 1) - camera_corners.first.x.get_double(offset, 1),
    camera_corners.second.y.get_double(offset, 1) - camera_corners.first.y.get_double(offset, 1)
  );

  va.Bind();
  ib.Bind();
  glLineWidth(10);
  glDrawElements(GL_LINES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}

template <typename T>
void OpenGLRenderer<T>::clear_screen() {
  glClear(GL_COLOR_BUFFER_BIT);
}