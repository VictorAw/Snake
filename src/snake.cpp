#include "snake.h"
#include "state.h"

namespace game
{

Snake::Snake(size_t x, size_t y, size_t length)
  : forward_axis_(Snake::Axis::Y),
    forward_sign_(1),
    movement_period_(0.1f),
    time_since_last_movement_(0.0f),
    expand_(false)
{
  Position body_segment { x, y };
  positions_.push_back(body_segment);

  for (size_t i=1; i<length; ++i)
  {
    body_segment.increment_axis(forward_axis_, -forward_sign_);
    positions_.push_back(body_segment);
    body_positions_.insert(positions_.back());
  }
}

Snake::Position const & Snake::head()
{
  return positions_.front();
}

std::set<Snake::Position> const & Snake::body()
{
  return body_positions_;
}

bool Snake::self_intersecting()
{
  return body_positions_.find(positions_.front()) != body_positions_.end();
}

void Snake::update(State & state)
{
  time_since_last_movement_ += state.delta_time;
  KeyboardState const & keyboard = state.input.keyboard;

  if (keyboard.left_pressed || keyboard.a_pressed)
  {
    turn_left();
  }
  else if (keyboard.right_pressed || keyboard.d_pressed)
  {
    turn_right();
  }
  else if (keyboard.up_pressed || keyboard.w_pressed)
  {
    turn_up();
  }
  else if (keyboard.down_pressed || keyboard.d_pressed)
  {
    turn_down();
  }

  if (time_since_last_movement_ > movement_period_)
  {
    move_forward();
    time_since_last_movement_ = 0.0f;
  }
}

void Snake::move_forward()
{
  Position head = positions_.front();
  head.increment_axis(forward_axis_, forward_sign_);
  body_positions_.insert(positions_.front());
  positions_.push_front(head);
 
  if (!expand_) 
  {
    body_positions_.erase(positions_.back());
    positions_.pop_back();
  }
  else
  {
    expand_ = false;
  }
}

void Snake::turn_left()
{
  if (forward_axis_ == Axis::Y)
  {
    forward_sign_ = -1;
  }
  forward_axis_ = Axis::X;
}

void Snake::turn_right()
{
  if (forward_axis_ == Axis::Y)
  { 
    forward_sign_ = 1;
  }
  forward_axis_ = Axis::X;
}

void Snake::turn_up()
{
  if (forward_axis_ == Axis::X)
  {
    forward_sign_ = 1;
  }
  forward_axis_ = Axis::Y;
}

void Snake::turn_down()
{
  if (forward_axis_ == Axis::X)
  {
    forward_sign_ = -1;
  }
  forward_axis_ = Axis::Y;
}

void Snake::expand()
{
  expand_ = true;
}

void Snake::Position::increment_axis(
  Snake::Axis const & axis, 
  int const & sign)
{
  if (axis == Snake::Axis::X)
  {
    this->x += sign;
  }
  else
  {
    this->y += sign;
  }
}

bool Snake::Position::operator < (Snake::Position const & other) const
{
  return (this->x == other.x ? this->y < other.y : this->x < other.x);
}

} // game

