#pragma once

#include <iostream>
#include <glad/glad.h>

using namespace std;

void openglErrorHandlingCallback(GLenum source,
                                           GLenum type,
                                           GLuint id,
                                           GLenum severity,
                                           GLsizei length,
                                           const GLchar* message,
                                           const void* userParam);
