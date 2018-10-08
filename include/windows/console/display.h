#ifndef DISPLAY_H
#define DISPLAY_H

#include "state.h"

#include <windows.h>

namespace game
{

  void clear_screen(HANDLE console_id);
  void init_display(State & state);
  void display(State & state);

} // game

#endif //DISPLAY_H
