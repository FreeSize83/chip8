#include "../include/Chip8.h"
#include <fstream>
#include <cstdint>

Chip8::Chip8() {
	reset();
	loadGame("game/pong.ch8");
}

void Chip8::reset() {
		for (int i = 0; i < 4096; i++){
			memory[i] = 0;
		}
		for (int i = 0; i < 16; i++) {
			V[i] = 0;
		}
		for (int i = 0; i < 16; i++) {
			stack[i] = 0;
		}
		for (int i = 0; i < 2048; i++) {
			display[i] = 0;
		}
		I = 0;
		PC = 0;
		SP = 0;
		delayTimer = 0;
		soundTimer = 0;
}

void Chip8::loadGame(const char* filename) {
	std::ifstream file(filename, std::ios::binary | std::ios::ate);
	std::streamsize size = file.tellg();

	if (!file.is_open()) return; // открылся ли файл 

	if (size > 3584) return; // если файл слишком большой

	file.seekg(0, std::ios::beg);
	file.read(reinterpret_cast<char*>(&memory[0x200]), size);
	file.close();
}

