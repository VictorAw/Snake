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
  previous_x_ = x_;
  previous_y_ = y_;

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

