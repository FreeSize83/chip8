# CHIP-8 Emulator (C++/raylib)

Эмулятор CHIP-8 для курсовой работы. Дисциплина разработки: Объектно-ориентированное программирование.

![screenshot](docs/screenshot.png)

## Сборка (Windows, CMake + Visual Studio)
1. Установить CMake, MSVC (VS 2022), git.
2. Склонировать repo и собрать:

   git clone <repo-url> chip8
   cd chip8
   cmake -S . -B out/build/x64-debug -DCMAKE_BUILD_TYPE=Debug
   cmake --build out/build/x64-debug --config Debug


## Запуск:

    powershell
   .\chip8.exe (Resolve-Path ..\..\..\game\pong.ch8) 12
   