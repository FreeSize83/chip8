#pragma once
#include "Memory.h"
#include <string>

struct RomLoader {
  static bool load(const std::string& path, Memory& mem);
};
