#include "../include/RaylibDisplay.h"
#include "raylib.h"

RaylibDisplay::RaylibDisplay(int scale) : m_scale(scale) {
	SetTargetFPS(60);
}

RaylibDisplay::~RaylibDisplay() {
	if (IsKeyPressed(KEY_F12)) TakeScreenshot("docs/screenshot.png");
}

void RaylibDisplay::clear() {
	BeginDrawing();
	ClearBackground(BLACK);
	EndDrawing(); 
}

void RaylibDisplay::present(const FramebufferView& fb, int scale) {
	BeginDrawing();
	ClearBackground(BLACK);

	for (int y = 0; y < FramebufferView::H; ++y) {
		for (int x = 0; x < FramebufferView::W; ++x) {
			if (fb.data[y * FramebufferView::W + x]) {
				DrawRectangle(x * m_scale, y * m_scale, m_scale, m_scale, RAYWHITE);
			}
		}
	}
	EndDrawing();
}
IDisplay* CreateRaylibDisplay(int scale) { return new RaylibDisplay(scale); }