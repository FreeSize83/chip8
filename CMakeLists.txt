cmake_minimum_required (VERSION 3.8)
project(chip8 CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#источники
add_executable(chip8
  src/main.cpp
  src/chip8.cpp
  src/RaylibDisplay.cpp
  include/Chip8.h
  include/display.h
  include/RaylibDisplay.h
)

file(GLOB_RECURSE ALL_CXX_SOURCE_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
)

find_program(CLANG_FORMAT_EXE NAMES clang-format)
if (CLANG_FORMAT_EXE)
  add_custom_target(format
    COMMAND ${CLANG_FORMAT_EXE} -i ${ALL_CXX_SOURCE_FILES}
    COMMENT "Running clang-format"
  )
endif()

target_include_directories(chip8 PRIVATE include)

#raylib
include(FetchContent)
FetchContent_Declare(raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG 5.0
)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(raylib)
target_link_libraries(chip8 PRIVATE raylib)

# консольное окно на Release
if (MSVC)
  target_compile_definitions(chip8 PRIVATE _CRT_SECURE_NO_WARNINGS)
else()
	target_compile_options(chip8 PRIVATE -Wall -Wextra -Wpedantic)
endif()