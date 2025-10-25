#pragma once
#include <array>
#include <cstdint>
#include <stdexcept>

class Stack {
public:
  void reset() {
    data.fill(0);
    sp = 0;
  }
  void bindSP(uint8_t* extSP) {
    ext_sp = extSP;
  } // связка с регистром SP
  void push(uint16_t a) {
    uint8_t& S = ext_sp ? *ext_sp : sp;
    if (S >= 16)
      throw std::runtime_error("Stack overflow");
    data[S++] = a;
  }
  uint16_t pop() {
    uint8_t& S = ext_sp ? *ext_sp : sp;
    if (S == 0)
      throw std::runtime_error("Stack underflow");
    return data[--S];
  }

private:
  std::array<uint16_t, 16> data{};
  uint8_t sp{0};
  uint8_t* ext_sp{nullptr};
};
