#include <cstdint>

class Chip8 {
	static constexpr int MEM_SIZE = 4096;
	static constexpr int FB_W = 64;
	static constexpr int FB_H = 32;
	static constexpr int STACK_SIZE = 16;
	static constexpr uint16_t ROM_BASE = 0x200;

	uint16_t stack[STACK_SIZE]{};
	uint8_t SP = 0; // size стэка

	inline bool push(uint16_t addr) {
		if (SP >= STACK_SIZE) return false;
		stack[SP++] = addr;
		return true;
	}

	inline bool pop(uint16_t& out) {
		if (SP == 0) return false;
		out = stack[--SP];
		return true;
	}

public:
	Chip8();
	void reset();
	void loadGame(const char* filename);
	void emulateCycle();
	void updateTimers();

private:
	uint8_t memory[4096];
	uint8_t V[16];
	uint16_t I;
	uint16_t PC;
	uint16_t stack[16];
	uint16_t SP;
	uint8_t delayTimer;
	uint8_t soundTimer;
	uint8_t display[64 * 32];
	uint8_t key[16];
};
