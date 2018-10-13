#include <windows/console/input.h>
#include <windows.h>

#include "state.h"

namespace game
{

void init_input(State & state)
{
  HANDLE console_id = GetStdHandle(STD_INPUT_HANDLE);
  state.input.console_id = console_id;

  state.input.keyboard.left_pressed = false;
  state.input.keyboard.right_pressed = false;
  state.input.keyboard.up_pressed = false;
  state.input.keyboard.down_pressed = false;
  state.input.keyboard.w_pressed = false;
  state.input.keyboard.a_pressed = false;
  state.input.keyboard.s_pressed = false;
  state.input.keyboard.d_pressed = false;

  DWORD mode;
  GetConsoleMode(console_id, &mode);
  mode &= ~ENABLE_PROCESSED_INPUT;
  SetConsoleMode(console_id, mode);
}

void handle_key_event(State & state, INPUT_RECORD const & event)
{
  KeyboardState & keyboard = state.input.keyboard;

  switch (event.Event.KeyEvent.wVirtualKeyCode)
  {
    case VK_LEFT:
      keyboard.left_pressed = event.Event.KeyEvent.bKeyDown;
      break;
    case VK_RIGHT:
      keyboard.right_pressed = event.Event.KeyEvent.bKeyDown;
      break;
    case VK_DOWN:
      keyboard.down_pressed = event.Event.KeyEvent.bKeyDown;
      break;
    case VK_UP:
      keyboard.up_pressed = event.Event.KeyEvent.bKeyDown;
      break;
    case 0x57: // W
      keyboard.w_pressed = event.Event.KeyEvent.bKeyDown;
      break;
    case 0x41: // A
      keyboard.a_pressed = event.Event.KeyEvent.bKeyDown;
      break;
    case 0x53: // S
      keyboard.s_pressed = event.Event.KeyEvent.bKeyDown;
      break;
    case 0x44: // D
      keyboard.d_pressed = event.Event.KeyEvent.bKeyDown;
      break;
    case 0x43: // C
      int ctrl_key_state = event.Event.KeyEvent.dwControlKeyState;
      int ctrl_pressed = ctrl_key_state & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED);
      if (event.Event.KeyEvent.bKeyDown && ctrl_pressed)
      {
        // Exit the game immediately
        state.game_over = true;
        state.running = false;
      }
      break;
  }
}

void read_input(State & state)
{
  HANDLE console_id = state.input.console_id;
  INPUT_RECORD & event = state.input.event_buffer;
  unsigned long int events;

  PeekConsoleInput(console_id, &(state.input.event_buffer), 1, &events);
  if (events > 0)
  {
    ReadConsoleInput(console_id, &(state.input.event_buffer), 1, &events);
    handle_key_event(state, state.input.event_buffer);
  }
}

} // game

