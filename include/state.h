#ifndef STATE_H
#define STATE_H

#include "snake.h"
#include "fruit.h"

#ifdef _WIN32
  #include <windows.h>
#ifdef CONSOLE
  #include "windows/console/input.h"
#endif // CONSOLE
#endif // _WIN32

#include <string>
#include <chrono>

namespace game
{

struct State
{
  //========================
  // Variables
  
  bool running;
  bool game_over;

  float frame_rate; // Hz
  float frame_period; // Seconds
  float delta_time; // Seconds

  size_t board_width;
  size_t board_height;
  size_t initial_snake_size;

  Snake::Ptr snake;
  Fruit::Ptr fruit;

  struct Display
  {
    #ifdef _WIN32
    #ifdef CONSOLE
      HANDLE console_id;  
    #else
      HANDLE window_id;
    #endif // CONSOLE
    #endif // _WIN32

    #ifdef CONSOLE
      std::wstring message;
      std::wstring board;

      size_t row_stride;
    #endif

  } display;

  struct Input
  {
    KeyboardState keyboard;

    #ifdef _WIN32
    #ifdef CONSOLE
      HANDLE console_id;
      INPUT_RECORD event_buffer;
    #else
      HANDLE window_id;
    #endif // COSNOLE
    #endif // _WIN32
  } input;

  //========================
  // Functions
  
  State();
};

} // game

#endif // STATE_H
