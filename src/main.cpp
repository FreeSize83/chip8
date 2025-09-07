#include "../include/Chip8.h"
#include "../include/RaylibDisplay.h"
#include "../include/display.h"
#include "raylib.h"
#include <chrono>
#include <thread>
#include <iostream>
#include <memory>
#include <string>
#include <cmath>

extern IDisplay* CreateRaylibDisplay(int scale);

class Beeper {
	AudioStream stream{};
	bool initialized = false;
	float phase = 0.f;
	const int sampleRate = 44100;
	const float freq = 440.0f;

public:
	Beeper() {
		InitAudioDevice();
		stream = LoadAudioStream(sampleRate, 16, 1);
		SetAudioStreamVolume(stream, 0.25f);
		initialized = true;
	}
	~Beeper() {
		if (initialized) {
			UnloadAudioStream(stream);
			CloseAudioDevice();
		}
	}
	void update(bool on) {
		if (!initialized) return;

		if (on) {
			if (!IsAudioStreamPlaying(stream)) PlayAudioStream(stream);

			const int SAMPLES = 512;
			static short buf[SAMPLES];

			while (IsAudioStreamProcessed(stream)) {
				for (int i = 0; i < SAMPLES; ++i) {
					float s = std::sinf(2.0f * 3.1415926535f * freq * phase);
					buf[i] = static_cast<short>(s * 3000);
					phase += 1.0f / sampleRate;
					if (phase >= 1.0f) phase -= 1.0f;
				}
				UpdateAudioStream(stream, buf, SAMPLES);
			}
		} 
		else {
			if (IsAudioStreamPlaying(stream)) StopAudioStream(stream);
		}
	}
};

void pollKeys_Raylib(Chip8& c8) { // клавиатура
	c8.key[0x1] = IsKeyDown(KEY_ONE);
	c8.key[0x2] = IsKeyDown(KEY_TWO);
	c8.key[0x3] = IsKeyDown(KEY_THREE);
	c8.key[0xC] = IsKeyDown(KEY_FOUR);

	c8.key[0x4] = IsKeyDown(KEY_Q);
	c8.key[0x5] = IsKeyDown(KEY_W);
	c8.key[0x6] = IsKeyDown(KEY_E);
	c8.key[0xD] = IsKeyDown(KEY_R);

	c8.key[0x7] = IsKeyDown(KEY_A);
	c8.key[0x8] = IsKeyDown(KEY_S);
	c8.key[0x9] = IsKeyDown(KEY_D);
	c8.key[0xE] = IsKeyDown(KEY_F);

	c8.key[0xA] = IsKeyDown(KEY_Z);
	c8.key[0x0] = IsKeyDown(KEY_X);
	c8.key[0xB] = IsKeyDown(KEY_C);
	c8.key[0xF] = IsKeyDown(KEY_V);
}


struct AppConfig {
	std::string romPath = "game/pong.ch8";
		int scale = 12;
		double cpuHz = 600.00;

};

static void printUsage(const char* exe) {
	std::cout <<
		"Usage:\n"
		"  " << exe << " [--rom <path>] [--scale <4..32>] [--cpu-hz <60..2000>]\n"
		"  " << exe << " <rom>\n\n"
		"Examples:\n"
		"  " << exe << " --rom game/ibm_logo.ch8 --scale 14 --cpu-hz 700\n"
		"  " << exe << " game/pong.ch8\n";
}

static AppConfig parseArgs(int argc, char** argv) {
	AppConfig cfg;
	for (int i = 1; i < argc; ++i) {
		std::string a = argv[i];
		if (a == "--help" || a == "--h") {
			printUsage(argv[0]);
			std::exit(0);
		}
		else if (a == "--rom" && i + 1 < argc) {
			cfg.romPath = argv[++i];
		}
		else if (a == "--scale" && i + 1 < argc) {
			try {
				cfg.scale = std::stoi(argv[++i]);
			}
			catch (...){}
		}
		else if (a == "--cpu-hz" && i + 1 < argc) {
			try {
				cfg.cpuHz = std::stod(argv[++i]);
			}
			catch (...){}
		}
		else if (!a.empty() && a[0] != '-') {
			cfg.romPath = a;
		}
		else {
			std::cerr << "Unknown option: " << a << "\n";
			printUsage(argv[0]);
			std::exit(1);
		}
	}
	if (cfg.scale < 4) cfg.scale = 4;
	if (cfg.scale > 32) cfg.scale = 32;
	if (cfg.cpuHz < 60.0) cfg.cpuHz = 60.0;
	if (cfg.cpuHz > 5000.0) cfg.cpuHz = 5000.0;
	return cfg;
}


int main(int argc, char** argv) {
	const char* romPath = (argc > 1) ? argv[1] : "game/pong.ch8";

	int scale = 12;
	if (argc > 2) {
		try { scale = std::stoi(argv[2]); } catch(...){}
		if (scale < 4) {
			scale = 4;
		}
		if (scale > 32) {
			scale = 32;
		}
	}

	AppConfig cfg = parseArgs(argc, argv);

	InitWindow(64 * scale, 32 * scale, "CHIP-8");
	SetTargetFPS(60);

	Beeper beep;
	std::unique_ptr<IDisplay> display(CreateRaylibDisplay(scale));

	Chip8 chip8;
	chip8.reset();
	chip8.loadGame(romPath);
	
	double CPU_HZ = 600.0;
	const double TIMER_HZ = 60.0;

	const double CPU_HZ = cfg.cpuHz;
	const double TIMER_GZ = 60.0;
	using clock = std::chrono::steady_clock;
	auto prev = clock::now();
	double accCpu = 0.0;
	double accTimer = 0.0;
	const double CPU_DT = 1.0 / CPU_HZ;
	const double TMR_DT = 1.0 / TIMER_HZ;

	bool running = true;
	bool paused = false;

	while (!WindowShouldClose()) { // Главный цикл
		auto now = clock::now();
		double dt = std::chrono::duration<double>(now - prev).count();
		prev = now;
		if (dt > 0.25) dt = 0.25;

		pollKeys_Raylib(chip8); // ввод

		if (IsKeyPressed(KEY_P)) {
			paused = !paused;
			std::cout << (paused ? "[paused]\n" : "[resume]\n");
		}

		if (IsKeyPressed(KEY_R)) {
			chip8.reset();
			chip8.loadGame(cfg.romPath.c_str());
			std::cout << "[reset]\n";
		}

		if (IsKeyPressed(KEY_ESCAPE)) break;

		if (!paused) {
			accCpu += dt;
			while (accCpu >= CPU_DT) {
				chip8.emulateCycle();
				accCpu -= CPU_DT;
			}
			accTimer += dt;
			while (accTimer >= TMR_DT) {
				chip8.updateTimers();
				accTimer -= TMR_DT;
			}
		}
		else {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}

		beep.update(chip8.soundOn());

		if (chip8.consumeDirty()) { // рендер
			FramebufferView fb{ chip8.framebufferData() };
			display->present(fb, scale);                   
		}
	}
	
	CloseWindow();
	return 0;
}
