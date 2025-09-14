#pragma once

#include <helpers/Position.h>
#include <Shapes/Shapes.h>
#include <Input/IZoomObserver.h>
#include <Input/IPointerUpObserver.h>
#include <Input/IConfirmObserver.h>
#include <Input/IClearObserver.h>
#include <Input/IZoomResetObserver.h>
#include <RendererDataProvider/IRendererDataProvider.h>

template<typename T>
class IRecursiveRenderer : public IRendererDataProvider<T>, public IZoomObserver<T>, public IPointerUpObserver, public IConfirmObserver, public IClearObserver, public IZoomResetObserver
{
public:
  virtual ~IRecursiveRenderer() = default;
};