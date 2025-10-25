#include "OpcodeExecutor.h"
#include "Chip8.h"
#include <stdexcept>

static inline uint8_t hi(uint16_t op) {
  return static_cast<uint8_t>((op & 0x0F00) >> 8);
}
static inline uint8_t lo(uint16_t op) {
  return static_cast<uint8_t>((op & 0x00F0) >> 4);
}

ExecResult OpcodeExecutor::execute(uint16_t op, Chip8& c8) {
  ExecResult R{};
  uint8_t x = hi(op);
  uint8_t y = lo(op);
  uint8_t n = static_cast<uint8_t>(op & 0x000F);
  uint8_t kk = static_cast<uint8_t>(op & 0x00FF);
  uint16_t nnn = static_cast<uint16_t>(op & 0x0FFF);

  auto& V = c8.regs.V;
  auto& I = c8.regs.I;
  auto& PC = c8.regs.PC;

  switch (op & 0xF000) {
  case 0x0000:
    if (op == 0x00E0) {
      c8.fb.clear();
    } else if (op == 0x00EE) {
      PC = c8.stack.pop();
      PC += 2;
      R.jumped = true;
    }
    break;

  case 0x1000:
    PC = nnn;
    R.jumped = true;
    break; 
  case 0x2000:
    c8.stack.push(PC);
    PC = nnn;
    R.jumped = true;
    break; 
  case 0x3000:
    PC += (V[x] == kk) ? 2 : 0;
    break; 
  case 0x4000:
    PC += (V[x] != kk) ? 2 : 0;
    break; 
  case 0x5000:
    if ((op & 0x000F) == 0)
      PC += (V[x] == V[y]) ? 2 : 0;
    break; 
  case 0x6000:
    V[x] = kk;
    break; 
  case 0x7000:
    V[x] = static_cast<uint8_t>(V[x] + kk);
    break; 

  case 0x8000: {
    switch (op & 0x000F) {
    case 0x0:
      V[x] = V[y];
      break; 
    case 0x1:
      V[x] |= V[y];
      V[0xF] = 0;
      break; 
    case 0x2:
      V[x] &= V[y];
      V[0xF] = 0;
      break; 
    case 0x3:
      V[x] ^= V[y];
      V[0xF] = 0;
      break; 
    case 0x4: {
      uint16_t s = V[x] + V[y];
      V[0xF] = s > 0xFF;
      V[x] = static_cast<uint8_t>(s);
      break;
    } 
    case 0x5: {
      V[0xF] = (V[x] >= V[y]);
      V[x] = static_cast<uint8_t>(V[x] - V[y]);
      break;
    } 
    case 0x6: {
      V[0xF] = V[x] & 0x1;
      V[x] >>= 1;
      break;
    } 
    case 0x7: {
      V[0xF] = (V[y] >= V[x]);
      V[x] = static_cast<uint8_t>(V[y] - V[x]);
      break;
    } 
    case 0xE: {
      V[0xF] = (V[x] >> 7) & 0x1;
      V[x] <<= 1;
      break;
    } 
    }
  } break;

  case 0x9000:
    if ((op & 0x000F) == 0)
      PC += (V[x] != V[y]) ? 2 : 0;
    break; 
  case 0xA000:
    I = nnn;
    break; 
  case 0xB000:
    PC = static_cast<uint16_t>((nnn + V[0]) & 0x0FFF);
    R.jumped = true;
    break; 
  case 0xC000:
    V[x] = static_cast<uint8_t>(c8.rand8() & kk);
    break;       
  case 0xD000: { 
    bool col = c8.fb.drawSprite(c8.mem.raw(), I, V[x], V[y], n);
    V[0xF] = col ? 1 : 0;
  } break;

  case 0xE000:
    if ((op & 0x00FF) == 0x9E) {
      if (c8.kbd.isDown(V[x]))
        PC += 2;
    } 
    else if ((op & 0x00FF) == 0xA1) {
      if (!c8.kbd.isDown(V[x]))
        PC += 2;
    } 
    break;

  case 0xF000:
    switch (op & 0x00FF) {
    case 0x07:
      V[x] = c8.timers.dt;
      break; 
    case 0x0A: {
      int k = c8.kbd.anyPressed();
      if (k < 0) {
        R.holdPC = true;
        return R;
      }
      V[x] = static_cast<uint8_t>(k);
    } break;
    case 0x15:
      c8.timers.dt = V[x];
      break; 
    case 0x18:
      c8.timers.st = V[x];
      break; 
    case 0x1E: {
      uint16_t s = c8.regs.I + V[x];
      c8.regs.I = s & 0x0FFF;
      V[0xF] = (s > 0x0FFF);
    } break; 
    case 0x29:
      c8.regs.I = static_cast<uint16_t>(0x50 + (V[x] & 0x0F) * 5);
      break; 
    case 0x33: {
      uint8_t v = V[x];
      c8.mem.write8(c8.regs.I, v / 100);
      c8.mem.write8(c8.regs.I + 1, (v / 10) % 10);
      c8.mem.write8(c8.regs.I + 2, v % 10);
    } break; 
    case 0x55: {
      for (uint8_t r = 0; r <= x; ++r)
        c8.mem.write8(c8.regs.I + r, c8.regs.V[r]);
    } break; 
    case 0x65: {
      for (uint8_t r = 0; r <= x; ++r)
        c8.regs.V[r] = c8.mem.read8(c8.regs.I + r);
    } break; 
    }
    break;

  default:
    break;
  }

  return R;
}
