#ifndef SNAKE_H
#define SNAKE_H

#include <chrono>
#include <memory>
#include <deque>
#include <set>

namespace game
{

class State;

class Snake
{
public:
  using Ptr = std::shared_ptr<Snake>;
  using RefPtr = std::weak_ptr<Snake>;

  enum class Axis { X=0, Y };

  struct Position
  {
  public:
    Position();
    Position(size_t x, size_t y, bool valid);

    size_t x;
    size_t y;

    void increment_axis(Snake::Axis const & axis, int const & sign);
    bool operator < (Position const & other) const;
    bool valid() const;
   
  private: 
    bool valid_;

    friend class Snake;
  };

  Snake(size_t x, size_t y, size_t length);

  void update(State & state);
  bool self_intersecting(); 
  void expand();

  Position const & head();
  std::set<Position> const & body();

  Position const & removed_position();

private:
  //==========================
  // Functions
  
  void move_forward();
  void turn_left();
  void turn_right();
  void turn_up();
  void turn_down();

  //==========================
  // Variables
  
  // Positions of each body segment
  std::deque<Position> positions_; 
  std::set<Position> body_positions_;

  // Diff of the current state after forward movement
  Position removed_position_;

  Axis forward_axis_; // X/Y
  int forward_sign_; // Positive/Negative

  float time_since_last_movement_; // Seconds
  float movement_period_; // Seconds

  bool expand_;
};

} // game

#endif // SNAKE_H
