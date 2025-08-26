#include <cstdint>

class Chip8 {
public:
	Chip8();
	void reset();
	void loadgame();
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
