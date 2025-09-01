#pragma once
#include "display.h"


class RaylibDisplay : public IDisplay {
public:
	explicit RaylibDisplay(int scale);
	~RaylibDisplay() override;
	void present(const FramebufferView& fb, int scale) override;
	void clear() override;
private:
	int m_scale;
};
IDisplay* CreateRaylibDisplay(int scale);