#include "OpenGLRenderer.h"

template <typename T>
OpenGLRenderer<T>::OpenGLRenderer(
  IRecursiveRenderer<T>* p_recursive_renderer,
  Configuration configuration
) : p_recursive_renderer(p_recursive_renderer), configuration(configuration) {

}

template <typename T>
void OpenGLRenderer<T>::render_to_screen() {

}