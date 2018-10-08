#ifndef GAME_H
#define GAME_H

#include "state.h"

#include <chrono>

namespace game
{
  using Time = std::chrono::high_resolution_clock;
  using seconds = std::chrono::duration<float>;

  void start();
  void request_game_dimensions(State & state);
  void run(State & state);

  void update_objects(State & state);
  void handle_collisions(State & state);

  bool is_number(std::string const &str);
}

#endif // GAME_H
