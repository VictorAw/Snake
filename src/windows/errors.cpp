#include "windows/errors.h"

#include <windows.h>
#include <iostream>

namespace game
{

  void print_error(std::wstring const & api_function_name)
  {
    std::wcout << __FILE__ << L":Error " << GetLastError()
              << L" from " << api_function_name << L" on line " << __LINE__
              << std::endl;
  }


} // game

