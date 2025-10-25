#include "Chip8.h"
#include "RomLoader.h"
#include <stdexcept>

Chip8::Chip8() {
  rng.seed(std::random_device{}());
  stack.bindSP(&regs.SP);
  reset();
}

void Chip8::reset() {
  mem.reset();
  regs.reset();
  stack.reset();
  timers.reset();
  fb.clear();
  kbd.clear();
}

bool Chip8::loadGame(const char* path) {
  reset();
  return RomLoader::load(path, mem);
}

void Chip8::emulateCycle() {
  const uint16_t pc = regs.PC;
  uint16_t opcode = (static_cast<uint16_t>(mem.read8(pc)) << 8) | mem.read8(pc + 1);
  ExecResult r = exec.execute(opcode, *this);
  if (!r.holdPC && !r.jumped)
    regs.PC += 2;
}

void Chip8::updateTimers() {
  timers.tick60Hz();
}
