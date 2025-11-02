#pragma once

#include <Renderer/OpenGLRenderer/OpenGLRenderer.h>
#include <Viewport/IWindowReconstructionObserver.h>
#include <RendererDataProvider/IRendererDataProvider.h>
#include <ACODataProvider/ACODataProvider.h>

template<typename T>
class ACOOpenGLRenderer : public IRenderer, public IWindowReconstructionObserver
{
private:
  OpenGLRenderer<T>* base_renderer;

  ACODataProvider<T>* p_data_provider;
  ICamera<T>* p_camera;
  Configuration configuration;
  std::list<IOnScreenButton*> on_screen_buttons;
  // VertexArray* vertex_array;
  // IndexBuffer* index_buffer;
  // Shader* basic_shader;
  // Shader* colored_shader;
  // double window_width = 960;
  // double window_height = 960;

  void render_shapes();
  void render_text();
  void render_buttons();
public:
  ACOOpenGLRenderer(ACODataProvider<T>* p_data_provider, ICamera<T>* p_camera, Configuration configuration, std::list<IOnScreenButton*> on_screen_buttons);
  ~ACOOpenGLRenderer() = default;
  void render_to_screen() override;
  void render_shapes_to_screen() override;
  void clear_screen() override;
  void process_window_resize(int width, int height) override;
  void process_window_reconstruction(IViewport* p_viewport) override;
  
  void render_aco_to_screen();
};