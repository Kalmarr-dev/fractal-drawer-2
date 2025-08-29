#include "OpenGLRenderer.h"

#include <iostream>
#include <math.h>
#include "../../helpers/Color.h"
#include "../../Shapes/IShape.h"
#include "../../Viewport/IViewport.h"

// #define _DEBUG

template <typename T>
OpenGLRenderer<T>::OpenGLRenderer(
  IRecursiveRenderer<T>* p_recursive_renderer,
  ICamera<T>* p_camera,
  Configuration configuration,
  std::list<IOnScreenButton*> on_screen_buttons
) : p_recursive_renderer(p_recursive_renderer), p_camera(p_camera), configuration(configuration),
    on_screen_buttons(on_screen_buttons)  {

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
void OpenGLRenderer<T>::render_buttons() {
  vector<float> positions;
  vector<unsigned int> indexes;

  const int floats_per_vertex = 5;
  
  for (auto &&p_button : on_screen_buttons)
  {
    Shapes<double> shapes = p_button->get_shapes(this->window_width, this->window_height);
    Color<double> color = p_button->get_color();
    for (auto &&p_shape : shapes.get_shapes())
    {
      int current_index = positions.size() / floats_per_vertex;
      for (auto &&i : p_shape->get_points())
      {
        positions.push_back((float)(i.x - 0.5) * 2);
        positions.push_back((float)(i.y - 0.5) * 2);
        positions.push_back(color.r);
        positions.push_back(color.g);
        positions.push_back(color.b);
      }
      for (auto &&i : p_shape->get_indexes())
      {
        indexes.push_back(current_index + i);
      }
    }
  }

  VertexBuffer vb(&positions[0], positions.size() * sizeof(float));
  IndexBuffer ib(&indexes[0], (unsigned int)indexes.size());

  VertexBufferLayout layout;
  layout.Push<float>(2);
  layout.Push<float>(3);

  VertexArray va;
  va.AddBuffer(vb, layout);

  this->colored_shader = new Shader("res/shaders/colored.shader");
  colored_shader->Bind();

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
  
  const auto& shapes_collection = shapes.get_shapes();

  #ifdef _DEBUG
    std::cout << shapes_collection.size() << '\n';
  #endif

  size_t shapes_collection_size = shapes_collection.size();
  #pragma omp parallel for schedule(static) if(10000 < shapes_collection_size)
  for (int i = 0; i < (int)shapes_collection.size(); i++)
  {
    auto shape = shapes_collection[i];
    auto shape_points = shape->get_points();
    auto shape_indexes = shape->get_indexes();
    double length_squared_log = std::log2(shape->get_linear_size_squared().get_double(offset_0, 0));
    double r = std::cos(length_squared_log * 0.15) * 0.5 + 0.5;
    double g = std::cos(length_squared_log * 0.1 + + 3.1415 * 0.5) * 0.5 + 0.5;
    double b = std::cos(length_squared_log * 0.25 + 3.1415) * 0.5 + 0.5;
    for (int j = 0; j < (int)shape_points.size(); j++)
    {
      auto point = shape_points[j];
      // std::cout << point.x.get_double(offset_0, 0) << '\n';
      T offset_point_x = (point.x - *offset_x);
      // std::cout << offset_point_x.get_double(offset_0, 0) << " " << width.get_double(offset_0, 0) << '\n';
      T scaled_point_x = offset_point_x / width;
      // std::cout << scaled_point_x.get_double(offset_0, 0) << '\n';
      T rescaled_point_x = (scaled_point_x - T(0.5)) * T(2.0);
      // std::cout << rescaled_point_x.get_double(offset_0, 0) << '\n';
      positions[i * floats_per_vertex * 2 + j * floats_per_vertex] = (float)(rescaled_point_x).get_double(offset_0, 0);
      // std::cout << point.y.get_double(offset_0, 0) << '\n';
      T offset_point_y = (point.y - *offset_y);
      // std::cout << offset_point_y.get_double(offset_0, 0) << " " << height.get_double(offset_0, 0) << '\n';
      T scaled_point_y = offset_point_y / height;
      // std::cout << scaled_point_y.get_double(offset_0, 0) << '\n';
      T rescaled_point_y = (scaled_point_y - T(0.5)) * T(2.0);
      // std::cout << rescaled_point_y.get_double(offset_0, 0) << '\n';
      positions[i * floats_per_vertex * 2 + j * floats_per_vertex + 1] = (float)(rescaled_point_y).get_double(offset_0, 0);
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

  render_buttons();
}

template <typename T>
void OpenGLRenderer<T>::render_rectangles_to_screen() {
  auto camera_corners = p_camera->get_camera_corners();
  T* offset_0 = new T(0);
  T* offset_x = new T(camera_corners.first.x);
  T* offset_y = new T(camera_corners.first.y);
  T width = camera_corners.second.x - camera_corners.first.x;
  T height = camera_corners.second.y - camera_corners.first.y;

  Shapes<T> shapes = p_recursive_renderer->get_shapes_on_camera();
  // vector sizes are subject to change
  const int floats_per_vertex = 6;
  vector<float> positions(shapes.get_shapes().size() * 4 * floats_per_vertex);
  vector<unsigned int> indexes(shapes.get_shapes().size() * 6);
  
  
  const auto& shapes_collection = shapes.get_shapes();

  #ifdef _DEBUG
    std::cout << shapes_collection.size() << '\n';
  #endif

  size_t shapes_collection_size = shapes_collection.size();
  #pragma omp parallel for schedule(static) if(10000 < shapes_collection_size)
  for (int i = 0; i < (int)shapes_collection.size(); i++)
  {
    auto shape = shapes_collection[i];
    auto shape_points = shape->get_points();
    auto shape_indexes = shape->get_indexes();
    double length_squared_log = std::log2(shape->get_linear_size_squared().get_double(offset_0, 0));
    double r = std::cos(length_squared_log * 0.15) * 0.5 + 0.5;
    double g = std::cos(length_squared_log * 0.1 + + 3.1415 * 0.5) * 0.5 + 0.5;
    double b = std::cos(length_squared_log * 0.25 + 3.1415) * 0.5 + 0.5;
    for (int j = 0; j < (int)shape_points.size(); j++)
    {
      auto point = shape_points[j];
      T offset_point_x = (point.x - *offset_x);
      T scaled_point_x = offset_point_x / width;
      T rescaled_point_x = (scaled_point_x - T(0.5)) * T(2.0);
      T offset_point_y = (point.y - *offset_y);
      T scaled_point_y = offset_point_y / height;
      T rescaled_point_y = (scaled_point_y - T(0.5)) * T(2.0);
      positions[i * floats_per_vertex * 4 + j * floats_per_vertex] = (float)(rescaled_point_x).get_double(offset_0, 0);
      positions[i * floats_per_vertex * 4 + j * floats_per_vertex + 1] = (float)(rescaled_point_y).get_double(offset_0, 0);
      positions[i * floats_per_vertex * 4 + j * floats_per_vertex + 2] = (float)shape->get_depth().get_double(offset_0, 0);
      positions[i * floats_per_vertex * 4 + j * floats_per_vertex + 3] = (float)r;
      positions[i * floats_per_vertex * 4 + j * floats_per_vertex + 4] = (float)g;
      positions[i * floats_per_vertex * 4 + j * floats_per_vertex + 5] = (float)b;
    }
    for (int j = 0; j < (int)shape_indexes.size(); j++)
    {
      indexes[i * 6 + j] = i * 4 + shape_indexes[j];
    }
  }

  VertexBuffer vb(&positions[0], positions.size() * sizeof(float));
  IndexBuffer ib(&indexes[0], (unsigned int)indexes.size());

  VertexBufferLayout layout;
  layout.Push<float>(3);
  layout.Push<float>(3);

  VertexArray va;
  va.AddBuffer(vb, layout);

  // TODO not doing this every render removes colors somehow
  // TODO can't delete shader??
  // delete this->colored_shader;
  this->colored_shader = new Shader("res/shaders/colored_depth.shader");
  colored_shader->Bind();

  colored_shader->SetUniform4f
  ( 
    "u_camera", -1.0, -1.0, 2.0, 2.0
  );

  va.Bind();
  ib.Bind();
  // glLineWidth(5);
  glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);

  render_buttons();

  delete offset_0;
  delete offset_x;
  delete offset_y;
}

template <typename T>
void OpenGLRenderer<T>::clear_screen() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

template <typename T>
void OpenGLRenderer<T>::process_window_resize(int window_width, int window_height) {
  glViewport(0, 0, window_width, window_height);
  this->window_width = window_width;
  this->window_height = window_height;
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glDepthRange(0.0, 1.0);
}

template <typename T>
void OpenGLRenderer<T>::process_window_reconstruction(IViewport* p_viewport) {
  std::pair<int, int> size = p_viewport->get_size();
  process_window_resize(size.first, size.second);
}
