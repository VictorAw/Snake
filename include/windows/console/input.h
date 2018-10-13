#ifndef INPUT_H
#define INPUT_H

namespace game
{
  struct State;

  struct KeyboardState
  {
    bool w_pressed;
    bool a_pressed;
    bool s_pressed;
    bool d_pressed;
    bool up_pressed;
    bool left_pressed;
    bool right_pressed;
    bool down_pressed;
  };

  void init_input(State & state);
  void read_input(State & state);
  void cleanup_input(State & state);

} // game

#endif // INPUT_H
