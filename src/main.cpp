#include "../include/Chip8.h"
#include <chrono>
#include <thread>
#include <iostream>

int main(int argc, char** argv) {
	const char* romPath = (argc > 1) ? argv[1] : "game/pong.ch8";

	Chip8 chip8;
	chip8.reset();
	chip8.loadGame(romPath);

	double CPU_HZ = 600.0;
	const double TIMER_HZ = 60.0;

	using clock = std::chrono::steady_clock;
	auto prev = clock::now();
	double accCpu = 0.0;
	double accTimer = 0.0;
	const double CPU_DT = 1.0 / CPU_HZ;
	const double TMR_DT = 1.0 / TIMER_HZ;

	bool running = true;
	bool paused = false;

	while (running) { // Главный цикл
		auto now = clock::now();
		double dt = std::chrono::duration<double>(now - prev).count();
		prev = now;
		if (dt > 0.25) dt = 0.25;

		if (!paused) {
			accCpu += dt;
			while (accCpu >= CPU_DT) {
				chip8.emulateCycle();
				accCpu -= CPU_DT;
			}
			accTimer += dt;
			while (accTimer >= TMR_DT) {
				chip8.updateTimers();
				accTimer -= TMR_DT;
			}
		}
		else {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	
	return 0;
}
