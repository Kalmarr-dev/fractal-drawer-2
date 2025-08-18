#pragma once

#include "../../../include/glad/glad.h"
#include "../IRenderer.h"
#include "../../Configuration/Configuration.h"
#include "../../RecursiveRenderer/IRecursiveRenderer.h"
#include "../../helpers/glErrorHandling.h"

template<typename T>
class OpenGLRenderer : public IRenderer
{
private:
  IRecursiveRenderer<T>* p_recursive_renderer;
  Configuration configuration;

  void render_shapes();
  void render_text();
  void render_buttons();
public:
  OpenGLRenderer(IRecursiveRenderer<T>* p_recursive_renderer, Configuration configuration);
  ~OpenGLRenderer() = default;
  void render_to_screen() override;
  void clear_screen() override;
};