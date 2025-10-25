#pragma once
#include <array>
#include <cstddef>
#include <cstdint>

class Memory {
public:
  static constexpr std::size_t SIZE = 4096;
  Memory();
  void reset();
  uint8_t read8(uint16_t a) const;
  void write8(uint16_t a, uint8_t v);
  uint8_t* raw() {
    return mem.data();
  }
  const uint8_t* raw() const {
    return mem.data();
  }

private:
  std::array<uint8_t, SIZE> mem{};
  void loadFontset();
};
