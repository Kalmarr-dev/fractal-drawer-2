#include "OpenGLRenderer.h"

#include <iostream>
#include <math.h>

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
  auto camera_corners = p_camera->get_camera_corners();
  T* offset_0 = new T(0);
  T* offset_x = new T(camera_corners.first.x);
  T* offset_y = new T(camera_corners.first.y);
  T width = camera_corners.second.x - camera_corners.first.x;
  T height = camera_corners.second.y - camera_corners.first.y;

  Shapes<T> shapes = p_recursive_renderer->get_shapes_on_camera();
  // vector sizes are subject to change
  const int floats_per_vertex = 5;
  vector<float> positions(shapes.get_shapes().size() * 2 * floats_per_vertex);
  vector<unsigned int> indexes(shapes.get_shapes().size() * 2);
  
  auto shapes_collection = shapes.get_shapes();

  std::cout << shapes_collection.size() << '\n';

  #pragma omp parallel for
  for (int i = 0; i < shapes_collection.size(); i++)
  {
    auto shape = shapes_collection[i];
    auto shape_points = shape->get_points();
    auto shape_indexes = shape->get_indexes();
    double length_squared_log = std::log2(shape->get_linear_size_squared().get_double(offset_0, 1));
    double r = std::cos(length_squared_log * 0.15) * 0.5 + 0.5;
    double g = std::cos(length_squared_log * 0.1 + + 3.1415 * 0.5) * 0.5 + 0.5;
    double b = std::cos(length_squared_log * 0.25 + 3.1415) * 0.5 + 0.5;
    for (int j = 0; j < shape_points.size(); j++)
    {
      auto point = shape_points[j];
      positions[i * floats_per_vertex * 2 + j * floats_per_vertex] = (float)(((point.x - *offset_x) / width - 0.5) * 2.0).get_double(offset_0, 1);
      positions[i * floats_per_vertex * 2 + j * floats_per_vertex + 1] = (float)(((point.y - *offset_y) / height - 0.5) * 2.0).get_double(offset_0, 1);
      positions[i * floats_per_vertex * 2 + j * floats_per_vertex + 2] = (float)r;
      positions[i * floats_per_vertex * 2 + j * floats_per_vertex + 3] = (float)g;
      positions[i * floats_per_vertex * 2 + j * floats_per_vertex + 4] = (float)b;

      // positions[i * 4 + j * 2] = (float)point.x.get_double(offset_0, 1);
      // positions[i * 4 + j * 2 + 1] = (float)point.y.get_double(offset_0, 1);
    }
    for (auto &&j : shape_indexes)
    {
      indexes[i * 2 + j] = i * 2 + j;
    }
  }

  VertexBuffer vb(&positions[0], positions.size() * sizeof(float));
  IndexBuffer ib(&indexes[0], (unsigned int)indexes.size());

  VertexBufferLayout layout;
  layout.Push<float>(2);
  layout.Push<float>(3);

  VertexArray va;
  va.AddBuffer(vb, layout);

  // TODO not doing this every render removes colors somehow
  // TODO can't delete shader??
  // delete this->colored_shader;
  this->colored_shader = new Shader("res/shaders/colored.shader");
  colored_shader->Bind();

  // colored_shader->SetUniform4f
  // ( 
  //   "u_camera", camera_corners.first.x.get_double(offset_0, 1),
  //   camera_corners.first.y.get_double(offset_0, 1),
  //   camera_corners.second.x.get_double(&camera_corners.first.x, 1),
  //   camera_corners.second.y.get_double(&camera_corners.first.y, 1)
  // );
  colored_shader->SetUniform4f
  ( 
    "u_camera", -1.0, -1.0, 2.0, 2.0
  );

  va.Bind();
  ib.Bind();
  glLineWidth(5);
  glDrawElements(GL_LINES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}

template <typename T>
void OpenGLRenderer<T>::clear_screen() {
  glClear(GL_COLOR_BUFFER_BIT);
}

template <typename T>
void OpenGLRenderer<T>::process_window_resize(int window_width, int window_height) {
  glViewport(0, 0, window_width, window_height);
}