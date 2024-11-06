#include "SFG/Own2dEngine/Utils/moreChrono.h"

namespace SFG {
namespace Own2dEngine {
namespace Utils {

std::chrono::secondsLongDouble getDurationSinceLast( std::chrono::high_resolution_clock::time_point& start ) {
  std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
  std::chrono::secondsLongDouble difference = std::chrono::duration_cast< std::chrono::secondsLongDouble >( now - start );
  start = now;
  return difference;
}

}  // namespace Utils
}  // namespace Own2dEngine
}  // namespace SFG
