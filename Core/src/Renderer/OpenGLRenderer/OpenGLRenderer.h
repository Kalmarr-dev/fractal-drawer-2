#pragma once

#include <list>
#ifndef __EMSCRIPTEN__
#include <glad/glad.h>
#else
#include <glad_es/glad.h>
#endif
#include "../IRenderer.h"
#include "../../Configuration/Configuration.h"
#include "../../helpers/glErrorHandling.h"
#include "../../Camera/ICamera.h"
#include "../../GLBuffers/VertexArray.h"
#include "../../GLBuffers/IndexBuffer.h"
#include "../../GLShaders/Shader.h"
#include "../../OnScreenButton/IOnScreenButton.h"
#include "../../Viewport/IWindowReconstructionObserver.h"
#include "../../RendererDataProvider/IRendererDataProvider.h"

template<typename T>
class OpenGLRenderer : public IRenderer, public IWindowReconstructionObserver
{
private:
  IRendererDataProvider<T>* p_data_provider;
  ICamera<T>* p_camera;
  Configuration configuration;
  std::list<IOnScreenButton*> on_screen_buttons;
  VertexArray* vertex_array;
  IndexBuffer* index_buffer;
  Shader* basic_shader;
  Shader* colored_shader;
  double window_width = 960;
  double window_height = 960;

  void render_shapes();
  void render_text();
  void render_buttons();
public:
  OpenGLRenderer(IRendererDataProvider<T>* p_data_provider, ICamera<T>* p_camera, Configuration configuration, std::list<IOnScreenButton*> on_screen_buttons);
  ~OpenGLRenderer();
  void render_to_screen() override;
  void render_shapes_to_screen() override;
  void clear_screen() override;
  void process_window_resize(int width, int height) override;
  void process_window_reconstruction(IViewport* p_viewport) override;
  
};