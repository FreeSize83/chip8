#pragma once

#include <array>
#include <cstdint>

struct Framebyffer {
	static constexpr int W = 64, H = 32;
	std::array<uint8_t, W * H> pix{};
};

class IDisplay {
public:
	virtual ~IDisplay() = default;
	virtual void clear() = 0;
	virtual void present(const Framebyffer& fb, int scale) = 0;
};