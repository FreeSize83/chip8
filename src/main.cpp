#include "Chip8.h"
#include "ConsoleDisplay.h"
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

int main(int argc, char** argv) {
  std::string rom = (argc > 1) ? argv[1] : "game/ibm_logo.ch8";
  int scale = (argc > 2) ? std::max(1, std::stoi(argv[2])) : 2;
  double CPU_HZ = (argc > 3) ? std::stod(argv[3]) : 600.0;

  Chip8 c8;
  if (!c8.loadGame(rom.c_str())) {
    std::cerr << "Failed to load ROM: " << rom << "\n";
    return 1;
  }

  using clock = std::chrono::steady_clock;
  const double TMR_HZ = 60.0;
  const double CPU_DT = 1.0 / CPU_HZ;
  const double TMR_DT = 1.0 / TMR_HZ;

  auto prev = clock::now();
  double accCpu = 0.0, accTmr = 0.0;

  std::cout << "\x1b[2J\x1b[?25l";

  while (true) {
    auto now = clock::now();
    double dt = std::chrono::duration<double>(now - prev).count();
    prev = now;
    if (dt > 0.25)
      dt = 0.25;

    accCpu += dt;
    while (accCpu >= CPU_DT) {
      c8.emulateCycle();
      accCpu -= CPU_DT;
    }
    accTmr += dt;
    while (accTmr >= TMR_DT) {
      c8.updateTimers();
      accTmr -= TMR_DT;
    }

    ConsoleDisplay::present(c8.framebufferData(), scale);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  std::cout << "\x1b[?25h";
  return 0;
}
