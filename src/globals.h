#pragma once

#include <fmt/core.h>

// Including SDL headers
#include <SDL2/SDL.h>
//#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_mixer.h>

#define SPDLOG_FMT_EXTERNAL
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/spdlog.h>

// library headers
#include <cstdint>
#include <chrono>
using namespace std::chrono_literals;
namespace std {
    namespace chrono {
        typedef duration<long double> secondsLongDouble;
    }
}

#include "utils/smartpointer.h"

static const uint16_t gridWidth = 32;
