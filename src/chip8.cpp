#include "../include/Chip8.h"
#include <fstream>
#include <cstdint>

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

void Chip8::loadGame(const char* filename) {
	std::ifstream file(filename, std::ios::binary | std::ios::ate);
	std::streamsize size = file.tellg();

	if (!file.is_open()) return; // открылся ли файл 

	if (size > 3584) return; // если файл слишком большой

	file.seekg(0, std::ios::beg);
	file.read(reinterpret_cast<char*>(&memory[0x200]), size);
	file.close();
}

void Chip8::emulateCycle() {
	uint16_t opcode = memory[PC] << 8 | memory[PC + 1];

	uint16_t first = opcode & 0xF000;
	uint16_t NNN = opcode & 0x0FFF;
	uint8_t NN = opcode & 0x00FF;
	uint8_t N = opcode & 0x000F;
	uint8_t X = (opcode & 0x0F00) >> 8;
	uint8_t Y = (opcode & 0x00F0) >> 4;

	switch (first) {

		case 0x0000:
			switch (opcode)
			{
			case 0x00E0:
				for (int i = 0; i < 64 * 32; i++)
					display[i] = 0;
				PC += 2;
				break;
			case 0x00EE:
				PC = stack[--SP];
				PC += 2;
				break;
			}
			break;

		case 0x1000: 
			PC = NNN;
			break;
	
		case 0x2000: 
			stack[SP++] = PC;
			PC = NNN;
			break;
	
		case 0x3000: 
			PC += (V[X] == NN) ? 4 : 2;
			break;
	
		case 0x6000:
			V[X] = NN;
			PC += 2;
			break;
	
		case 0x7000:
			V[X] += NN;
			PC += 2;
			break;
		case 0xA000:
			I = NNN;
			PC += 2;
			break;
		case 0xD000:
			PC += 2;
			break;
		default:
			PC += 2;
			break;
}