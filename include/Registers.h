#pragma once
#include <array>
#include <cstdint>

class Registers {
public:
  Registers() {
    reset();
  }
  void reset() {
    V.fill(0);
    I = 0;
    PC = 0x200;
    SP = 0;
  }
  std::array<uint8_t, 16> V{};
  uint16_t I = 0;
  uint16_t PC = 0x200;
  uint8_t SP = 0;
};
