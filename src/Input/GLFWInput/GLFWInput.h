#pragma once

#include "GLFWKey.h";
#include "../InputSubject.h";
#include "../../helpers/Position.h";

class GLFWInput : public InputSubject 
{
private:
  Position pointer_position;
  GLFWKey fullscreen_key;
  GLFWKey zoom_reset_key;
  GLFWKey zoom_in_key;
  GLFWKey zoom_out_key;
  GLFWKey clear_key;
  GLFWKey lock_zoom_key;
public:
  GLFWInput(GLFWKey fullscreen_key, GLFWKey zoom_reset_key, GLFWKey zoom_in_key, GLFWKey zoom_out_key, GLFWKey clear_key, GLFWKey lock_zoom_key);
  ~GLFWInput();
  
  void on_pointer_move(Position position);
  void on_key_press(int key);
};
