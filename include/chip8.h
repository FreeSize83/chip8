#pragma once
#include "Framebuffer.h"
#include "Keypad.h"
#include "Memory.h"
#include "OpcodeExecutor.h"
#include "Registers.h"
#include "Stack.h"
#include "Timers.h"
#include <cstdint>
#include <random>

class Chip8 {
public:
  Chip8();
  void reset();
  bool loadGame(const char* path);
  void emulateCycle();
  void updateTimers();

  // доступ к состоянию
  const uint8_t* framebufferData() const {
    return fb.data();
  }
  uint8_t* framebufferData() {
    return fb.data();
  }
  bool soundOn() const {
    return timers.st > 0;
  }
  Keypad& keypad() {
    return kbd;
  }
  const Keypad& keypad() const {
    return kbd;
  }

  Memory mem;
  Registers regs;
  Stack stack;
  Timers timers;
  Framebuffer fb;
  Keypad kbd;

  uint8_t rand8() {
    return static_cast<uint8_t>(dist(rng));
  }

private:
  OpcodeExecutor exec;
  std::mt19937 rng;
  std::uniform_int_distribution<int> dist{0, 255};
};
