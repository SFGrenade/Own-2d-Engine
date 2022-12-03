#pragma once

// Including FMT headers
#include <fmt/chrono.h>
#include <fmt/core.h>

// Including SDL headers
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

// Including SPDLOG headers
#define SPDLOG_FMT_EXTERNAL
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

// library headers
#include <chrono>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <mutex>
#include <optional>
#include <string>
#include <thread>
#include <vector>
using namespace std::chrono_literals;
namespace std {
namespace chrono {
typedef duration<long double> secondsLongDouble;
}
}  // namespace std

#include "pugiXML/pugixml.hpp"

static const uint16_t gridWidth = 32;
