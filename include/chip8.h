#include <cstdint>
#include <random>

class Chip8 {
	static constexpr int MEM_SIZE = 4096;
	static constexpr int FB_W = 64;
	static constexpr int FB_H = 32;
	static constexpr int STACK_SIZE = 16;
	static constexpr uint16_t ROM_BASE = 0x200;

public:
	Chip8();
	void seed(uint32_t s);
	void reset();
	void loadGame(const char* filename);
	void emulateCycle();
	void updateTimers();

	uint8_t key[16]; // клавиатура 

	const uint8_t* framebufferData() const {
		return display;
	}
	bool consumeDirty() {
		bool was = dirty;
		dirty = false;
		return was;
	}
	
private:
	std::mt19937 rng;
	std::uniform_int_distribution<int> dist{ 0, 255 };
	inline uint8_t rand8() {
		return static_cast<uint8_t>(dist(rng));
	}
	uint8_t memory[MEM_SIZE];
	uint8_t V[16]{};
	uint16_t I = 0;
	uint16_t PC = ROM_BASE;
	uint16_t stack[STACK_SIZE];
	uint16_t SP = 0;
	uint8_t delayTimer = 0;
	uint8_t soundTimer = 0;
	uint8_t display[FB_W * FB_H]{};
	bool dirty = false;
};
