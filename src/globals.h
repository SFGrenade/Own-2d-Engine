#pragma once

#include <fmt/core.h>

// Including SDL headers
#include <SDL2/SDL.h>
//#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_mixer.h>

// library headers
#include <cstdint>
#include <chrono>
using namespace std::chrono_literals;
namespace std {
    namespace chrono {
        typedef duration<long double> secondsLongDouble;
    }
}

static const uint16_t gridWidth = 32;
