#pragma once

#include <array>
#include <cstdint>

struct framebyffer {
	static constexpr int W = 64, H = 32;
	std::array<uint8_t, W* H> pix{};
};

class IDisplay {
	public:
		virtual ~IDisplay() = default;
		virtual void clear() = 0;
		virtual void present(const framebyffer& fb, int scale) = 0;
};