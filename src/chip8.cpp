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
		for (int i = 0; i < 16; i++) {
			key[i] = 0;
		}
		

		I = 0;
		PC = 0x200;
		SP = 0;
		delayTimer = 0;
		soundTimer = 0;
		
		const uint8_t chip8_fontset[80] = {
		 0xF0, 0x90, 0x90, 0x90, 0xF0, 
		 0x20, 0x60, 0x20, 0x20, 0x70, 
		 0xF0, 0x10, 0xF0, 0x80, 0xF0, 
		 0xF0, 0x10, 0xF0, 0x10, 0xF0, 
		 0x90, 0x90, 0xF0, 0x10, 0x10, 
		 0xF0, 0x80, 0xF0, 0x10, 0xF0, 
		 0xF0, 0x80, 0xF0, 0x90, 0xF0, 
		 0xF0, 0x10, 0x20, 0x40, 0x40, 
		 0xF0, 0x90, 0xF0, 0x90, 0xF0, 
		 0xF0, 0x90, 0xF0, 0x10, 0xF0, 
		 0xF0, 0x90, 0xF0, 0x90, 0x90, 
		 0xE0, 0x90, 0xE0, 0x90, 0xE0, 
		 0xF0, 0x80, 0x80, 0x80, 0xF0, 
		 0xE0, 0x90, 0x90, 0x90, 0xE0, 
		 0xF0, 0x80, 0xF0, 0x80, 0xF0, 
		 0xF0, 0x80, 0xF0, 0x80, 0x80  
		};

		for (int i = 0; i < 80; i++) {
			memory[0x50 + i] = chip8_fontset[i];
		}

}

void Chip8::loadGame(const char* filename) {
	std::ifstream file(filename, std::ios::binary | std::ios::ate);
	if (!file.is_open()) return; // открылся ли файл 

	const std::streamsize size = file.tellg();
	if (size <= 0 || size > (4096 - 0x200)) return; // если файл слишком большой

	file.seekg(0, std::ios::beg);
	if (!file.read(reinterpret_cast <char*> (memory[0x200]), size)) return;
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
