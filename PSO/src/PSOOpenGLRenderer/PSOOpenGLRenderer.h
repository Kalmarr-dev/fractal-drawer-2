#pragma once

#include <Renderer/OpenGLRenderer/OpenGLRenderer.h>
#include <Viewport/IWindowReconstructionObserver.h>
#include <RendererDataProvider/IRendererDataProvider.h>
#include <PSODataProvider/PSODataProvider.h>

template<typename T>
class PSOOpenGLRenderer : public IRenderer, public IWindowReconstructionObserver
{
private:
  OpenGLRenderer<T>* base_renderer;

  PSODataProvider<T>* p_data_provider;
  ICamera<T>* p_camera;
  Configuration configuration;
  std::list<IOnScreenButton*> on_screen_buttons;

  void render_shapes();
  void render_text();
  void render_buttons();
public:
  PSOOpenGLRenderer(PSODataProvider<T>* p_data_provider, ICamera<T>* p_camera, Configuration configuration, std::list<IOnScreenButton*> on_screen_buttons);
  ~PSOOpenGLRenderer() = default;
  void render_to_screen() override;
  void render_shapes_to_screen() override;
  void clear_screen() override;
  void process_window_resize(int width, int height) override;
  void process_window_reconstruction(IViewport* p_viewport) override;
  
  void render_pso_to_screen();
};