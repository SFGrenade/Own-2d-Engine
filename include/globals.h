#pragma once

// library headers
#include <chrono>
#include <string>
#include <vector>

using namespace std::chrono_literals;
namespace std {
namespace chrono {
typedef duration< long double > secondsLongDouble;
}
}  // namespace std

std::vector< std::string > str_split( std::string s, std::string delim );
