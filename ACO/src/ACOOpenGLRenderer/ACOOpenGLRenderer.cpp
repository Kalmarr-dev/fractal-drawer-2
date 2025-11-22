#include <ACOOpenGLRenderer/ACOOpenGLRenderer.h>

template<typename T>
ACOOpenGLRenderer<T>::ACOOpenGLRenderer(
  ACODataProvider<T>* p_data_provider, ICamera<T>* p_camera,
  Configuration configuration, std::list<IOnScreenButton*> on_screen_buttons
) : p_data_provider(p_data_provider), p_camera(p_camera),
  configuration(configuration), on_screen_buttons(on_screen_buttons) 
{
  this->base_renderer = new OpenGLRenderer(p_data_provider, p_camera, configuration, on_screen_buttons);
}

template<typename T>
void ACOOpenGLRenderer<T>::render_to_screen() {
  base_renderer->render_to_screen();
}

template<typename T>
void ACOOpenGLRenderer<T>::render_shapes_to_screen() {
  base_renderer->render_shapes_to_screen();
}

template<typename T>
void ACOOpenGLRenderer<T>::clear_screen() {
  base_renderer->clear_screen();
}

template<typename T>
void ACOOpenGLRenderer<T>::process_window_resize(int width, int height) {
  base_renderer->process_window_resize(width, height);
}

template<typename T>
void ACOOpenGLRenderer<T>::process_window_reconstruction(IViewport* p_viewport) {
  base_renderer->process_window_reconstruction(p_viewport);
}

template<typename T>
void ACOOpenGLRenderer<T>::render_aco_to_screen() {
  auto camera_corners = p_camera->get_camera_corners();
  T* offset_0 = new T(0);
  T* offset_x = new T(camera_corners.first.x);
  T* offset_y = new T(camera_corners.first.y);
  T width = camera_corners.second.x - camera_corners.first.x;
  T height = camera_corners.second.y - camera_corners.first.y;

  std::vector< Color<T> > colors;
  Shapes<T> shapes = p_data_provider->get_aco_history(&colors);
  // vector sizes are subject to change
  const int floats_per_rectangle_vertex = 6;
  const int floats_per_line_vertex = 6;
  
  const auto& shapes_collection = shapes.get_shapes();
  std::vector<IShape<T>*> rectangles;
  std::vector<IShape<T>*> lines;
  std::vector<Color<T>> rectangle_colors;
  std::vector<Color<T>> line_colors;
  for (int i = 0; (size_t)i < shapes_collection.size(); i++)
  {
    auto shape = shapes_collection[i];
    auto color = colors[i];
    if (shape->get_type() == ShapeType::RECTANGLE)
    {
      rectangles.push_back(shape);
      rectangle_colors.push_back(color);
    } else if (shape->get_type() == ShapeType::LINE) {
      lines.push_back(shape);
      line_colors.push_back(color);
    }
  }
  
  vector<float> rectangle_positions(rectangles.size() * 4 * floats_per_rectangle_vertex);
  vector<unsigned int> rectangle_indexes(rectangles.size() * 6);
  vector<float> line_positions(lines.size() * 2 * floats_per_line_vertex);
  vector<unsigned int> line_indexes(lines.size() * 2);
  

  #ifdef _DEBUG
    std::cout << shapes_collection.size() << '\n';
  #endif

  size_t rectangles_size = rectangles.size();
  // std::cout << rectangles_size << std::endl;
  #pragma omp parallel for schedule(static) if(10000 < rectangles_size)
  for (int i = 0; i < (int)rectangles.size(); i++)
  {
    auto shape = rectangles[i];
    auto shape_points = shape->get_points();
    auto shape_indexes = shape->get_indexes();
    double r = rectangle_colors[i].r.get_double(offset_0, 0);
    double g = rectangle_colors[i].g.get_double(offset_0, 0);
    double b = rectangle_colors[i].b.get_double(offset_0, 0);
    for (int j = 0; j < (int)shape_points.size(); j++)
    {
      auto point = shape_points[j];
      T offset_point_x = (point.x - *offset_x);
      T scaled_point_x = offset_point_x / width;
      T rescaled_point_x = (scaled_point_x - T(0.5)) * T(2.0);
      T offset_point_y = (point.y - *offset_y);
      T scaled_point_y = offset_point_y / height;
      T rescaled_point_y = (scaled_point_y - T(0.5)) * T(2.0);
      rectangle_positions[i * floats_per_rectangle_vertex * 4 + j * floats_per_rectangle_vertex] = (float)(rescaled_point_x).get_double(offset_0, 0);
      rectangle_positions[i * floats_per_rectangle_vertex * 4 + j * floats_per_rectangle_vertex + 1] = (float)(rescaled_point_y).get_double(offset_0, 0);
      rectangle_positions[i * floats_per_rectangle_vertex * 4 + j * floats_per_rectangle_vertex + 2] = (float)shape->get_depth().get_double(offset_0, 0);
      rectangle_positions[i * floats_per_rectangle_vertex * 4 + j * floats_per_rectangle_vertex + 3] = (float)r;
      rectangle_positions[i * floats_per_rectangle_vertex * 4 + j * floats_per_rectangle_vertex + 4] = (float)g;
      rectangle_positions[i * floats_per_rectangle_vertex * 4 + j * floats_per_rectangle_vertex + 5] = (float)b;
    }
    for (int j = 0; j < (int)shape_indexes.size(); j++)
    {
      rectangle_indexes[i * 6 + j] = i * 4 + shape_indexes[j];
    }
  }
  size_t lines_size = lines.size();
  #pragma omp parallel for schedule(static) if(10000 < lines_size)
  for (int i = 0; i < (int)lines.size(); i++)
  {
    auto shape = lines[i];
    auto shape_points = shape->get_points();
    auto shape_indexes = shape->get_indexes();
    double r = line_colors[i].r.get_double(offset_0, 0);
    double g = line_colors[i].g.get_double(offset_0, 0);
    double b = line_colors[i].b.get_double(offset_0, 0);
    for (int j = 0; j < (int)shape_points.size(); j++)
    {
      auto point = shape_points[j];
      T offset_point_x = (point.x - *offset_x);
      T scaled_point_x = offset_point_x / width;
      T rescaled_point_x = (scaled_point_x - T(0.5)) * T(2.0);
      T offset_point_y = (point.y - *offset_y);
      T scaled_point_y = offset_point_y / height;
      T rescaled_point_y = (scaled_point_y - T(0.5)) * T(2.0);
      line_positions[i * floats_per_line_vertex * 2 + j * floats_per_line_vertex] = (float)(rescaled_point_x).get_double(offset_0, 0);
      line_positions[i * floats_per_line_vertex * 2 + j * floats_per_line_vertex + 1] = (float)(rescaled_point_y).get_double(offset_0, 0);
      line_positions[i * floats_per_line_vertex * 2 + j * floats_per_line_vertex + 2] = (float)shape->get_depth().get_double(offset_0, 0);
      line_positions[i * floats_per_line_vertex * 2 + j * floats_per_line_vertex + 3] = (float)r;
      line_positions[i * floats_per_line_vertex * 2 + j * floats_per_line_vertex + 4] = (float)g;
      line_positions[i * floats_per_line_vertex * 2 + j * floats_per_line_vertex + 5] = (float)b;
    }
    for (int j = 0; j < (int)shape_indexes.size(); j++)
    {
      line_indexes[i * 2 + j] = i * 2 + shape_indexes[j];
    }
  }

  VertexBuffer vb_rectangles(&rectangle_positions[0], rectangle_positions.size() * sizeof(float));
  IndexBuffer ib_rectangles(&rectangle_indexes[0], (unsigned int)rectangle_indexes.size());
  VertexBufferLayout layout_rectangles;
  layout_rectangles.Push<float>(3);
  layout_rectangles.Push<float>(3);
  VertexArray va_rectangles;
  va_rectangles.AddBuffer(vb_rectangles, layout_rectangles);

#ifndef __EMSCRIPTEN__
  auto colored_shader = new Shader("res/shaders/colored_depth.shader");
#else
  auto colored_shader = new Shader("res/shaders/colored_depth_webgl2.shader");
#endif
  colored_shader->Bind();
  colored_shader->SetUniform4f
  ( 
    "u_camera", -1.0, -1.0, 2.0, 2.0
  );

  va_rectangles.Bind();
  ib_rectangles.Bind();
  // glLineWidth(5);
  glDrawElements(GL_TRIANGLES, ib_rectangles.GetCount(), GL_UNSIGNED_INT, nullptr);

  VertexBuffer vb_lines(&line_positions[0], line_positions.size() * sizeof(float));
  IndexBuffer ib_lines(&line_indexes[0], (unsigned int)line_indexes.size());
  VertexBufferLayout layout_lines;
  layout_lines.Push<float>(3);
  layout_lines.Push<float>(3);
  VertexArray va_lines;
  va_lines.AddBuffer(vb_lines, layout_lines);

  colored_shader->Bind();
  colored_shader->SetUniform4f
  ( 
    "u_camera", -1.0, -1.0, 2.0, 2.0
  );

  va_lines.Bind();
  ib_lines.Bind();
  glLineWidth(3);
  glDrawElements(GL_LINES, ib_lines.GetCount(), GL_UNSIGNED_INT, nullptr);

  // TODO render_buttons();

  delete offset_0;
  delete offset_x;
  delete offset_y;
  delete colored_shader;
}
