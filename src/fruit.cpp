#include "fruit.h"

namespace game
{

Fruit::Fruit(size_t board_width, size_t board_height)
  : x_distribution_(0, board_width-1),
    y_distribution_(0, board_height-1)
{
  respawn();
}

void Fruit::respawn()
{
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

} // game

