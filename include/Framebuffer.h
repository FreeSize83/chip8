#pragma once
#include <array>
#include <cstdint>

class Framebuffer {
public:
  static constexpr int W = 64, H = 32;
  void clear() {
    pixels.fill(0);
  }
  // ¬озвращает true, если была коллизи€ (1->0) хот€ бы в одной точке
  bool drawSprite(const uint8_t* mem, uint16_t I, uint8_t x0, uint8_t y0, uint8_t n) {
    bool collided = false;
    const uint8_t x = x0 & 63;
    const uint8_t y = y0 & 31;
    for (uint8_t row = 0; row < n; ++row) {
      const uint8_t sprite = mem[I + row];
      const uint8_t yy = (y + row) & 31;
      for (uint8_t bit = 0; bit < 8; ++bit) {
        if (sprite & (0x80u >> bit)) {
          const uint8_t xx = (x + bit) & 63;
          const size_t idx = yy * W + xx;
          uint8_t prev = pixels[idx];
          uint8_t next = prev ^ 1;
          pixels[idx] = next;
          if (prev == 1 && next == 0)
            collided = true;
        }
      }
    }
    return collided;
  }
  const uint8_t* data() const {
    return pixels.data();
  }
  uint8_t* data() {
    return pixels.data();
  }

private:
  std::array<uint8_t, W * H> pixels{};
};
