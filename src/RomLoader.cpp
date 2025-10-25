#include "RomLoader.h"
#include <fstream>

bool RomLoader::load(const std::string& path, Memory& mem) {
  std::ifstream f(path, std::ios::binary | std::ios::ate);
  if (!f)
    return false;
  auto size = static_cast<std::size_t>(f.tellg());
  if (size == 0 || 0x200 + size > Memory::SIZE)
    return false;
  f.seekg(0, std::ios::beg);
  f.read(reinterpret_cast<char*>(mem.raw() + 0x200), static_cast<std::streamsize>(size));
  return static_cast<bool>(f);
}
