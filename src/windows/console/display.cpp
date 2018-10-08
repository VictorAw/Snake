#include "windows/console/display.h"
#include "windows/errors.h"

#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <cstddef>
#include <string>

namespace game
{

wchar_t constexpr SNAKE_BODY_CHAR = L'\x25CB';
wchar_t constexpr FRUIT_CHAR = L'\x25CC';

void clear_screen(HANDLE console_id)
{
  COORD cursor_home = { 0, 0 };

  BOOL success;
  DWORD chars_written;
  CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
  DWORD console_size;

  // Get the number of characters in the buffer
  success = GetConsoleScreenBufferInfo(console_id, &screen_buffer_info);
  if (!success) { print_error(L"GetConsoleScreenBufferInfo"); }
  COORD & screen_buffer_size = screen_buffer_info.dwSize;
  console_size = screen_buffer_size.X * screen_buffer_size.Y;

  // Fill the screen with blanks
  success = FillConsoleOutputCharacter(
    console_id, 
    (TCHAR)' ', 
    console_size, 
    cursor_home, 
    &chars_written
  );
  if (!success) { print_error(L"FillConsoleOutputCharacter"); }

  // Get the current text attribute
  success = GetConsoleScreenBufferInfo(console_id, &screen_buffer_info);
  if (!success) { print_error(L"GetConsoleScreenBufferInfo"); }

  // Set buffer attributes
  success = FillConsoleOutputAttribute(
    console_id,
    screen_buffer_info.wAttributes,
    console_size,
    cursor_home,
    &chars_written
  );
  if (!success) { print_error(L"FillConsoleOutputAttribute"); }

  // Put the cursor at (0, 0)
  success = SetConsoleCursorPosition(console_id, cursor_home);
  if (!success) { print_error(L"SetConsoleCursorPosition"); }
}

void set_cursor(HANDLE console_id, size_t row, size_t col)
{
  std::wcout.flush();
  COORD coord = { (SHORT)col, (SHORT)row };
  SetConsoleCursorPosition(console_id, coord);
}

void set_character(HANDLE console_id, size_t row, size_t col, wchar_t value)
{
  set_cursor(console_id, row, col);
  std::wcout << value;
}

void draw_top_border(
  HANDLE console_id,
  size_t row,
  size_t row_stride,
  size_t board_width)
{
  set_character(console_id, row, 0, L'\x2554');
  for (size_t i=0; i<board_width; ++i)
  {
    for (size_t j=1; j<4; ++j)
    {
      set_character(console_id, row, (i*4)+j, L'\x2550');
    }

    if (i < board_width-1)
    {
      set_character(console_id, row, (i*4)+4, L'\x2566');
    }
  }
  set_character(console_id, row, row_stride-1, L'\x2557');
}

void draw_empty_row(
  HANDLE console_id,
  size_t row,
  size_t row_stride,
  size_t board_width,
  bool border)
{
  wchar_t left_char = border ? L'\x2560' : L'\x2551';
  wchar_t cell_char = border ? L'\x2550' : L' ';
  wchar_t divider_char = border ? L'\x256C' : L'\x2551';
  wchar_t right_char = border ? L'\x2563' : L'\x2551';

  set_character(console_id, row, 0, left_char);
  for (size_t i=0; i<board_width; ++i)
  {
    for (size_t j=1; j<4; ++j)
    {
      set_character(console_id, row, (i*4)+j, cell_char);
    }

    if (i < board_width-1)
    {
      set_character(console_id, row, (i*4)+4, divider_char);
    }
  }
  set_character(console_id, row, row_stride-1, right_char);
}

void draw_bottom_border(
  HANDLE console_id,
  size_t total_row_count, 
  size_t row_stride,
  size_t board_width)
{
  set_character(console_id, total_row_count, 0, L'\x255A');
  for (size_t i=0; i<board_width; ++i)
  {
    for (size_t j=1; j<4; ++j)
    {
      set_character(console_id, total_row_count, (i*4)+j, L'\x2550');
    }
  
    if (i < board_width-1)
    {
      set_character(console_id, total_row_count, (i*4)+4, L'\x2569');
    }
  }
  set_character(console_id, total_row_count, row_stride-1, L'\x255D');
}

void init_display(State & state)
{
  // Enable utf16 printing
  _setmode(_fileno(stdout), _O_U16TEXT);
  if (std::wcout.fail())
  {
    std::wcout.clear();
  }

  HANDLE console_id = GetStdHandle(STD_OUTPUT_HANDLE);
  state.display.console_id = console_id;

  CONSOLE_CURSOR_INFO cursor_info;
  GetConsoleCursorInfo(console_id, &cursor_info);
  cursor_info.bVisible = false;
  SetConsoleCursorInfo(console_id, &cursor_info);

  size_t width = state.board_width;
  size_t height = state.board_height;
  // Cell: | o | o | o | etc.
  // (3 spaces per board column) + dividers + left border + right border 
  size_t row_stride = (width * 3) + (width+1);
  state.display.row_stride = row_stride;
  // All rows and their upper border, and then the final bottom border
  size_t rows = (height * 2) + 1;

  std::wstring & board = state.display.board;
  board.resize(row_stride * rows);

  clear_screen(console_id);
  std::wcout << std::wstring(width+1, L' ') << L"Welcome to Snek!\n";
  draw_top_border(console_id, 1, row_stride, width);
  for (size_t i=1; i<rows-1; ++i)
  {
    // Draw border between two rows
    bool border = i % 2 == 0;
    // Draw one row lower due to welcome message
    draw_empty_row(console_id, i+1, row_stride, width, border);
  }
  draw_bottom_border(console_id, rows, row_stride, width);
}

void update_cell(
  HANDLE console_id,
  size_t x, 
  size_t y, 
  size_t row_stride, 
  wchar_t value)
{
  // Top border + row + border above each row + welcome message
  size_t row = (2 * y) + 2;
  // (3 spaces per column) + dividers + initial two left spaces
  size_t col = 2 + (3 * x) + x;
  set_character(console_id, row, col, value);
  //board[jump_to_row + jump_to_col] = value;
}

void clear_board(State & state)
{
  HANDLE console_id = state.display.console_id;
  set_cursor(console_id, 0, 0);

  size_t width = state.board_width;
  size_t height = state.board_height;

  size_t row_stride = state.display.row_stride;
  std::wstring & board = state.display.board;

  for (size_t i=0; i<height; ++i)
  {
    for (size_t j=0; j<width; ++j)
    {
      update_cell(console_id, j, i, row_stride, ' ');
    }
  }
}

void update_board(State & state)
{
  HANDLE console_id = state.display.console_id;
  set_cursor(console_id, 0, 0);

  Snake::Ptr snake = state.snake;
  Fruit::Ptr fruit = state.fruit;
  size_t row_stride = state.display.row_stride;
  size_t y_max = state.board_height - 1;
  std::wstring & board = state.display.board;

  Snake::Position const & head = snake->head();
  // Adjust from y=0 being at the top of the board to being at the bottom
  size_t head_y = y_max - head.y;
  std::set<Snake::Position> const & body = snake->body();
  update_cell(console_id, head.x, head_y, row_stride, SNAKE_BODY_CHAR);
  for (Snake::Position const & body_segment : body)
  {
    // Adjust from y=0 being at the top of the board to being at the bottom
    size_t body_segment_y = y_max - body_segment.y;
    update_cell(
      console_id,
      body_segment.x, 
      body_segment_y,
      row_stride, 
      SNAKE_BODY_CHAR
    );
  }

  // Adjust from y=0 being at the top of the board to being at the bottom
  size_t fruit_y = y_max - fruit->y();
  update_cell(console_id, fruit->x(), fruit_y, row_stride, FRUIT_CHAR);
}

void display(State & state)
{
  if (state.game_over)
  {
    HANDLE console_id = state.display.console_id;
    size_t rows = (state.board_height * 2) + 2;
    set_cursor(console_id, rows, 0);
    std::wcout << L"Game over. Please try again.\n";
  }
  else
  {
    clear_board(state);
    update_board(state);
  }
}

} // game

