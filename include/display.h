#pragma once

#include <array>
#include <cstdint>

struct FramebufferView {
	const uint8_t* data;              
	static constexpr int W = 64, H = 32;
};

class IDisplay {
public:
	virtual ~IDisplay() = default;
	virtual void present(const FramebufferView& fb, int scale) = 0;
	virtual void clear() = 0;
};

IDisplay* CreateRaylibDisplay(int scale);