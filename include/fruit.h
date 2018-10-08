#ifndef FRUIT_H
#define FRUIT_H

#include <memory>
#include <random>

namespace game
{

class Fruit
{
public:
  using Ptr = std::shared_ptr<Fruit>;
  using RefPtr = std::weak_ptr<Fruit>;

  Fruit(size_t board_width, size_t board_height);

  size_t x();
  size_t y();
  void respawn();

private:
  size_t x_;
  size_t y_;

  std::uniform_int_distribution<size_t> x_distribution_;
  std::uniform_int_distribution<size_t> y_distribution_;
  std::random_device rng_;
};

} // game

#endif // FRUIT_H
