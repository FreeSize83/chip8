#pragma once
#include <cstdint>

class Chip8;

struct ExecResult {
  bool holdPC = false; 
  bool jumped = false;
};

class OpcodeExecutor {
public:
  ExecResult execute(uint16_t opcode, Chip8& c8);
};
