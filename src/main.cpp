#include "../include/Chip8.h"

int main() {
	Chip8 chip8;
	chip8.loadGame("game/pong.ch8");

	return 0;
}
