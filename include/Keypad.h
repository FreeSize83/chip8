#pragma once
#include <array>
#include <cstdint>

class Keypad {
public:
  void clear() {
    keys.fill(false);
  }
  void set(uint8_t k, bool down) {
    if (k < 16)
      keys[k] = down;
  }
  bool isDown(uint8_t k) const {
    return (k < 16) ? keys[k] : false;
  }
  int anyPressed() const {
    for (int i = 0; i < 16; ++i)
      if (keys[i])
        return i;
    return -1;
  }

private:
  std::array<bool, 16> keys{};
};
