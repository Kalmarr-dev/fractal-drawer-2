#pragma once

#include "../../../include/glad/glad.h"
#include "../IRenderer.h"
#include "../../Configuration/Configuration.h"
#include "../../RecursiveRenderer/IRecursiveRenderer.h"
#include "../../helpers/glErrorHandling.h"
#include "../../Camera/ICamera.h"
#include "../../GLBuffers/VertexArray.h"
#include "../../GLBuffers/IndexBuffer.h"
#include "../../GLShaders/Shader.h"

template<typename T>
class OpenGLRenderer : public IRenderer
{
private:
  IRecursiveRenderer<T>* p_recursive_renderer;
  ICamera<T>* p_camera;
  Configuration configuration;
  VertexArray* vertex_array;
  IndexBuffer* index_buffer;
  Shader* basic_shader;
  Shader* colored_shader;

  void render_shapes();
  void render_text();
  void render_buttons();
public:
  OpenGLRenderer(IRecursiveRenderer<T>* p_recursive_renderer, ICamera<T>* p_camera, Configuration configuration);
  ~OpenGLRenderer();
  void render_to_screen() override;
  void clear_screen() override;
  void process_window_resize(int width, int height) override;
};