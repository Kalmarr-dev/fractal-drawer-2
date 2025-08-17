#pragma once

#include "../../Configuration/Configuration.h";
#include "../../DataStructure2D/IDataStructure2D.h";
#include "../IRecursiveRenderer.h";
#include "../IRecursiveRendererObserver.h";

template<typename T = double>
class BasicRecursiveRenderer : public IRecursiveRenderer, public IRecursiveRendererObserver
{
private:
  IDataStructure2D<T>* data_structure;
  const Configuration configuration;

public:
  BasicRecursiveRenderer(IDataStructure2D<T>* data_structure, const Configuration& configuration);

  Shapes get_shapes_on_camera(Position<T> lower, Position<T> higher) override;

  virtual void zoom_in() = 0;
  virtual void zoom_out() = 0;
  virtual void zoom_reset() = 0;
  virtual void clear_shapes() = 0;
};