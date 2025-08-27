#include "../include/Chip8.h"

Chip8::Chip8() {
	reset();
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