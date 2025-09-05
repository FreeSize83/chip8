#include "../include/Chip8.h"
#include "../include/RaylibDisplay.h"
#include "../include/display.h"
#include "raylib.h"
#include <chrono>
#include <thread>
#include <iostream>
#include <memory>
#include <string>

extern IDisplay* CreateRaylibDisplay(int scale);

void pollKeys_Raylib(Chip8& c8) { // клавиатура
	c8.key[0x1] = IsKeyDown(KEY_ONE);
	c8.key[0x2] = IsKeyDown(KEY_TWO);
	c8.key[0x3] = IsKeyDown(KEY_THREE);
	c8.key[0xC] = IsKeyDown(KEY_FOUR);

	c8.key[0x4] = IsKeyDown(KEY_Q);
	c8.key[0x5] = IsKeyDown(KEY_W);
	c8.key[0x6] = IsKeyDown(KEY_E);
	c8.key[0xD] = IsKeyDown(KEY_R);

	c8.key[0x7] = IsKeyDown(KEY_A);
	c8.key[0x8] = IsKeyDown(KEY_S);
	c8.key[0x9] = IsKeyDown(KEY_D);
	c8.key[0xE] = IsKeyDown(KEY_F);

	c8.key[0xA] = IsKeyDown(KEY_Z);
	c8.key[0x0] = IsKeyDown(KEY_X);
	c8.key[0xB] = IsKeyDown(KEY_C);
	c8.key[0xF] = IsKeyDown(KEY_V);
}


int main(int argc, char** argv) {
	const char* romPath = (argc > 1) ? argv[1] : "game/pong.ch8";

	int scale = 12;
	if (argc > 2) {
		try { scale = std::stoi(argv[2]); } catch(...){}
		if (scale < 4) {
			scale = 4;
		}
		if (scale > 32) {
			scale = 32;
		}
	}

	InitWindow(64 * scale, 32 * scale, "CHIP-8");
	SetTargetFPS(60);
	std::unique_ptr<IDisplay> display(CreateRaylibDisplay(scale));

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

	while (!WindowShouldClose()) { // Главный цикл
		auto now = clock::now();
		double dt = std::chrono::duration<double>(now - prev).count();
		prev = now;
		if (dt > 0.25) dt = 0.25;

		pollKeys_Raylib(chip8); // ввод

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

		if (chip8.consumeDirty()) { // рендер
			FramebufferView fb{ chip8.framebufferData() };
			display->present(fb, scale);                   
			chip8.consumeDirty();
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	
	CloseWindow();
	return 0;
}
