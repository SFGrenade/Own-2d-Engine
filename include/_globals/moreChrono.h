#ifndef _GLOBALS_MORECHRONO_H_
#define _GLOBALS_MORECHRONO_H_

// library headers
#include <chrono>

using namespace std::chrono_literals;
namespace std {
namespace chrono {
typedef duration< long double > secondsLongDouble;
}
}  // namespace std

std::chrono::secondsLongDouble getDurationSinceLast( std::chrono::high_resolution_clock::time_point& start );

#endif /* _GLOBALS_MORECHRONO_H_ */
