#ifndef UTILS_MORECHRONO_H_
#define UTILS_MORECHRONO_H_

#include <chrono>

using namespace std::chrono_literals;
namespace std {
namespace chrono {
typedef duration< long double > secondsLongDouble;
}
}  // namespace std

namespace SFG {
namespace Own2dEngine {
namespace Utils {

std::chrono::secondsLongDouble getDurationSinceLast( std::chrono::high_resolution_clock::time_point& start );

}  // namespace Utils
}  // namespace Own2dEngine
}  // namespace SFG

#endif /* UTILS_MORECHRONO_H_ */
