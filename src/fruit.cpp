#include "fruit.h"

namespace game
{

Fruit::Fruit(size_t board_width, size_t board_height)
  : x_(0),
    y_(0),
    x_distribution_(0, board_width-1),
    y_distribution_(0, board_height-1)
{
  respawn();
}

void Fruit::respawn()
{
  respawn(true);
}

void Fruit::respawn(bool replace_history)
{
  if (replace_history)
  {
    previous_x_ = x_;
    previous_y_ = y_;
  }

  // For now, we just generate a random number, but we will want to in the
  // future be able to generate numbers with exclusion of certain coordinates
  //
  // The most straightforward algorithm is to generate a new distribution for
  // x in the range of the board.
  // Once the x is selected, generate a y value in the range of the board minus
  // any y values that are occuped at that x position. If all y values are
  // occupied, generate a new x position in the range of the board excluding
  // the last generated x value.
  x_ = x_distribution_(rng_);
  y_ = y_distribution_(rng_);
}

size_t Fruit::x()
{
  return x_;
}

size_t Fruit::y()
{
  return y_;
}

size_t Fruit::previous_x()
{
  return previous_x_;
}

size_t Fruit::previous_y()
{
  return previous_y_;
}

} // game

