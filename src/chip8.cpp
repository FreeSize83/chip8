#include "../include/Chip8.h"
#include <fstream>
#include <cstdint>

Chip8::Chip8() {
	std::random_device rd;
	rng.seed(rd());
	reset();
}

void Chip8::seed(uint32_t s) {
	rng.seed(s);
}

void Chip8::reset() {
		std::memset(memory, 0, sizeof(memory));
		std::memset(V, 0, sizeof(V));
		std::memset(stack, 0, sizeof(stack));
		std::memset(display, 0, sizeof(display));
		std::memset(key, 0, sizeof(key));

		I = 0;
		PC = ROM_BASE;
		SP = 0;
		delayTimer = 0;
		soundTimer = 0;
		
		const uint8_t font[80] = {
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

		std::memcpy(&memory[0x50], font, sizeof(font));

}

void Chip8::loadGame(const char* filename) {
	std::ifstream file(filename, std::ios::binary | std::ios::ate);
	if (!file.is_open()) return; // открылся ли файл 

	const std::streamsize size = file.tellg();
	if (size <= 0 || size > (4096 - 0x200)) return; // если файл слишком большой

	file.seekg(0, std::ios::beg);
	if (!file.read(reinterpret_cast <char*> (&memory[0x200]), size)) return;
}

void Chip8::updateTimers() {
	if (delayTimer > 0) --delayTimer;
	if (soundTimer > 0) --soundTimer;
}

void Chip8::emulateCycle() {
	if (PC > MEM_SIZE - 2) return;
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
			std::memset(display, 0, sizeof(display));
			dirty = true;
			PC += 2;
			break;
		case 0x00EE:
			if (SP == 0) {
				PC += 2;
				break;
			}
			PC = stack[--SP] + 2; // возврат на следующую
			break;
		default:
			PC += 2;
			break;
		}
		break;

	case 0x1000:
		PC = NNN;
		break;

	case 0x2000: {
		if (SP >= STACK_SIZE) {
			PC += 2;
			break;
		}
		stack[SP++] = PC;
		PC = NNN;
		break;
	}

	case 0x3000:
		PC += (V[X] == NN) ? 4 : 2;
		break;

	case 0x4000: {
		PC += (V[X] != NN) ? 4 : 2;
		break;
	}

	case 0x5000: {
		if ((opcode & 0x000F) == 0) {
			PC += (V[X] == V[Y]) ? 4 : 2;
		}
		else {
			PC += 2;
		}
		break;
	}

	case 0x6000:
		V[X] = NN;
		PC += 2;
		break;

	case 0x7000:
		V[X] += static_cast<uint8_t>(V[X] + NN);
		PC += 2;
		break;

	case 0x8000: {
		uint8_t& vx = V[X];
		uint8_t& vy = V[Y];

		switch (N) {
		case 0x0:
			vx = vy;
			PC += 2;
			break;

		case 0x1:
			vx |= vy;
			V[0xF] = 0;
			PC += 2;
			break;

		case 0x2:
			vx &= vy;
			V[0xF] = 0;
			PC += 2;
			break;

		case 0x3:
			vx ^= vy;
			V[0xF] = 0;
			PC += 2;
			break;

		case 0x4: {
			uint16_t sum = static_cast<uint16_t>(vx) + static_cast<uint16_t>(vy);
			V[0xF] = (sum > 0xFF) ? 1 : 0;
			vx = static_cast<uint8_t>(sum & 0xFF);
			PC += 2;
			break;
		}

		case 0x5: {
			V[0xF] = (vx >= vy) ? 1 : 0;
			uint16_t diff = static_cast<uint16_t>(vx) - static_cast<uint16_t>(vy);
			vx = static_cast<uint8_t>(diff & 0xFF);
			PC += 2;
			break;
		}

		case 0x6: {
			uint8_t lsb = vx & 0x01;
			vx >>= 1;
			V[0xF] = lsb;
			PC += 2;
			break;
		}

		case 0x7: {
			V[0xF] = (vy >= vx) ? 1 : 0;
			uint16_t diff = static_cast<uint16_t>(vy) - static_cast<uint16_t>(vx);
			vx = static_cast<uint8_t>(diff & 0xFF);
			PC += 2;
			break;
		}

		case 0xE: {
			uint8_t msb = (vx & 0x80) ? 1 : 0;
			vx = static_cast<uint8_t>((vx << 1) & 0xFF);
			V[0xF] = msb;
			PC += 2;
			break;
		}

		default:
			PC += 2;
			break;
		}

		break;
	}
	case 0x9000: 
		if ((opcode & 0x000F) == 0) {
			PC += (V[X] == V[Y]) ? 4 : 2;
		}
		else {
			PC += 2;
		}
		break;
	

	case 0xA000:
		I = NNN;
		PC += 2;
		break;

	case 0xB000: {
		PC = (NNN + V[0]) & 0x0FFF;
		break;
	}

	case 0xC000: {
		V[X] = static_cast<uint8_t>(rand8() & NN);
		PC += 2;
		break;
	}

	case 0xD000: {
		const uint8_t x0 = V[X] & 63; // ширина 
		const uint8_t y0 = V[Y] & 31; // высота

		V[0xF] = 0;

		if (I > MEM_SIZE - N) {
			PC += 2;
			break;
		}
		for (uint8_t row = 0; row < N; ++row) {
			uint8_t sprite = memory[I + row];
			uint8_t yy = (y0 + row) & 31;

			if (!sprite) continue;


			for (uint8_t bit = 0; bit < 8; ++bit) {
				if (sprite & (0x80u >> bit)) {
					const uint8_t xx = (x0 + bit) & 63;
					const size_t idx = static_cast<size_t>(yy) * 64 + xx;

					const uint8_t prev = display[idx];
					const uint8_t next = prev ^ 1;
					display[idx] = next;

					if (prev == 1 && next == 0) {
						V[0xF] = 1;
					}
				}
			}
		}
		dirty = true;
		PC += 2;
		break;
	}

	case 0xE000: {
		switch (opcode & 0x00FF) {
		case 0x009E: {
			uint8_t k = V[X] & 0x0F;
			PC += (key[k] ? 4 : 2);
			break;
		}
		case 0x00A1: {
			uint8_t k = V[X] & 0x0F;
			PC += (!key[k] ? 4 : 2);
			break;
		}
		default: // игнор опкодов
			PC += 2;
			break;
		}
		break;
	}

	case 0xF000: {
		switch (opcode & 0x00FF) {
		case 0x0007:
			V[X] = delayTimer;
			PC += 2;
			break;

		case 0x0015:
			delayTimer = V[X];
			PC += 2;
			break;

		case 0x0018:
			soundTimer = V[X];
			PC += 2;
			break;
			
		case 0x001E: {
			uint16_t sum = I + V[X];
			V[0xF] = (sum > 0x0FFF) ? 1 : 0;
			I = sum & 0x0FFF;
			PC += 2;
			break;
		}

		case 0x0033: {
			uint8_t v = V[X];
			memory[I] = v / 100;
			memory[I + 1] = (v / 10) % 10;
			memory[I + 2] = v % 10;
			PC += 2;
			break;
		}

		case 0x0055: {
			for (uint8_t r = 0; r <= X; ++r) memory[I + r] = V[r];
			PC += 2;
			break;
		}

		case 0x0065: {
			for (uint8_t r = 0; r <= X; ++r) V[r] = memory[I + r];
			PC += 2;
			break;
		}
		case 0x000A: {
			int pressed = -1;
			for (int i = 0; i < 16; ++i) {
				if (key[i]) {
					pressed = i;
					break;
				}
			}
			if (pressed < 0) {
				return;
			}
			else {
				V[X] = static_cast<uint8_t>(pressed);
				PC += 2;
			}
			break;
		}
		default:
			PC += 2;
			break;
		}
		break;
	}

	default:
		PC += 2;
		break;
	}
}