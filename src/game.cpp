#ifdef _WIN32
#ifdef CONSOLE
  #include "windows/console/display.h"
#else
#endif // CONSOLE
#endif // _WIN32

#include "state.h"
#include "game.h"

#include <thread>
#include <iostream>

namespace game
{

void start()
{
  State game_state;
  
  game_state.frame_rate = 60.0f;
  game_state.frame_period = 1.0f / game_state.frame_rate;
  
  request_game_dimensions(game_state);
  game_state.snake = std::make_shared<Snake>(
    game_state.board_width / 2,
    game_state.board_height / 2,
    game_state.initial_snake_size
  );
  game_state.fruit = std::make_shared<Fruit>(
    game_state.board_width,
    game_state.board_height
  );

  init_display(game_state);
  init_input(game_state);

  run(game_state);
}

bool is_number(std::wstring const &str)
{
  size_t size = str.size();
  for (size_t i=0; i<size; ++i)
  {
    if (str[i] < '0' || str[i] > '9')
    {
      return false;
    }
  }

  return true;
}

void request_game_dimensions(State & state)
{
  std::wstring input;
  while (1) 
  {
    std::wcout << L"What is the width of the board? ";
    std::wcin >> input;

    if (!is_number(input))
    {
      std::wcout << L"\nPlease enter a number.\n" << std::endl;
    }
    else
    {
      state.board_width = std::stol(input);
      break;
    }
  }

  while (1)
  {
    std::wcout << L"What is the height of the board? ";
    std::wcin >> input;

    if (!is_number(input))
    {
      std::wcout << L"\nPlease enter a number.\n" << std::endl;
    }
    else
    {
      state.board_height = std::stol(input);
      break;
    }
  }

  while (1)
  {
    std::wcout << L"What is the initial length of the snake? ";
    std::wcin >> input;

    if (!is_number(input))
    {
      std::wcout << L"\nPlease enter a number.\n" << std::endl;
    }
    else
    {
      state.initial_snake_size = std::stol(input);
      break;
    }
  }
}

void run(State & state)
{
  state.running = true;
  Time::time_point previous_frame_time = Time::now();

  while (state.running)
  {
    Time::time_point now = Time::now();
    state.delta_time = std::chrono::duration_cast<seconds>(
      Time::now() - previous_frame_time
    ).count();
    previous_frame_time = now;

    read_input(state);
    // Exit event was read from input, user wants to exit without finishing 
    // the game 
    if (!state.running) 
    {
      break;
    }

    update_objects(state);
    handle_collisions(state);
    display(state);

    std::this_thread::sleep_for(
      std::chrono::duration<float, std::ratio<1>>(state.frame_period)
    );
    
    if (state.game_over)
    {
      state.running = false;
    }
  }

  cleanup_display(state);
  cleanup_input(state);
}

void update_objects(State & state)
{
  state.snake->update(state);
}

bool out_of_bounds(size_t x, size_t y, size_t max_x, size_t max_y)
{
  return x > max_x || y > max_y;
}

void handle_collisions(State & state)
{
  Snake::Ptr snake = state.snake;
  Snake::Position const & head = snake->head();
  if (snake->self_intersecting()
   || out_of_bounds(
        head.x, head.y, state.board_width-1, state.board_height-1))
  {
    state.game_over = true;
    return;
  }

  Fruit::Ptr fruit = state.fruit;

  if (head.x == fruit->x() && head.y == fruit->y())
  {
    snake->expand();
    fruit->respawn();

    // For now, we will just constantly re-generate new numbers if the
    // selected position is occupied. Eventually we'll want to prevent
    // the selection of occupied positions
    bool replace_history = true;
    fruit->respawn(replace_history); // Update the stored previous position
    bool same_position = 
      fruit->x() == fruit->previous_x() && fruit->y() == fruit->previous_y();
    while (same_position || snake->contains(fruit->x(), fruit->y()))
    {
      fruit->respawn(!replace_history); 
    }
  }
}

} // game

