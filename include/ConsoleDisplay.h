#pragma once
#include <cstdint>

struct ConsoleDisplay {
  static void present(const uint8_t* fb, int scale = 1);
};
