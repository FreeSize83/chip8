#include "ConsoleDisplay.h"
#include <iostream>

void ConsoleDisplay::present(const uint8_t* fb, int scale) {
  const int W = 64, H = 32;
  std::cout << "\x1b[H"; // ANSI: курсор влево-вверх (можно отключить на Windows)
  for (int y = 0; y < H; ++y) {
    for (int s = 0; s < 1; ++s) { // по вертикали без масштабировани€, чтобы не раст€гивать консоль
      for (int x = 0; x < W; ++x) {
        char ch = fb[y * W + x] ? '\xDB' : ' '; // ? или пробел
        for (int k = 0; k < scale; ++k)
          std::cout << ch;
      }
      std::cout << "\n";
    }
  }
  std::cout.flush();
}
