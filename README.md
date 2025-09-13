# CHIP-8 Emulator (C++/raylib)

Эмулятор CHIP-8 для курсовой работы. Дисциплина разработки: структура кода, паттерны, тайминги, история коммитов.

![screenshot](D:/chip8/out/build/x64-debug/screenshot000.png)

## Возможности
- CPU (~600 Гц), таймеры 60 Гц.
- Графика 64×32, рисование (DXYN), VF=collision.
- Клавиатура 16 клавиш (1-2-3-4 / QWER / ASDF / ZXCV).
- Звук: тон, пока soundTimer > 0.
- CLI: --rom, --scale, --cpu-hz. Горячие: P (пауза), R (reset), Esc (выход).


## Сборка (Windows, CMake + Visual Studio)
1. Установить CMake, MSVC (VS 2022), git.
2. Склонировать repo и собрать:

   git clone <repo-url> chip8
   cd chip8
   cmake -S . -B out/build/x64-debug -DCMAKE_BUILD_TYPE=Debug
   cmake --build out/build/x64-debug --config Debug
````

3. Запуск:

   ```powershell
   .\out\build\x64-debug\chip8.exe --rom game\pong.ch8 --scale 12 --cpu-hz 600
   
- ## Управление и раскладка
CHIP-8:   1  2  3  C       4  5  6  D       7  8  9  E       A  0  B  F
Клавиши:  1  2  3  4       Q  W  E  R       A  S  D  F       Z  X  C  V
Горячие:  P — пауза, R — reset, Esc — выход