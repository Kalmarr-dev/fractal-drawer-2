#pragma once

#include "../IRenderer.h";
#include "../../Configuration/Configuration.h";
#include "../../RecursiveRenderer/IRecursiveRenderer.h";

template<typename T = double>
class OpenGLRenderer : public IRenderer
{
private:
  IRecursiveRenderer<T> recursive_renderer;
  Configuration configuration;

  void render_shapes();
  void render_text();
  void render_buttons();
public:
  OpenGLRenderer(IRecursiveRenderer<T>& recursive_renderer, Configuration configuration);
  void render_to_screen() override;
};