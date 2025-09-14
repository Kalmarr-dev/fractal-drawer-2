#pragma once

struct GLFWKey
{
  int code;
  bool is_pressed;
  int amount_of_times_clicked;

  GLFWKey(int code) {
    this->code = code;
    this->is_pressed = false;
    this->amount_of_times_clicked = 0;
  }
};
