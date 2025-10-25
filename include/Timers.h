#pragma once
#include <cstdint>

class Timers {
public:
  void reset(){ dt=0; st=0; }
  void tick60Hz(){
    if (dt>0) --dt;
    if (st>0) --st;
  }
  uint8_t dt{0}, st{0};
};
