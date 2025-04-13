#ifndef UTILS_NUMOPS_H_
#define UTILS_NUMOPS_H_

#include <algorithm>
#include <cmath>

namespace SFG {
namespace Own2dEngine {
namespace Utils {

template < typename T >
T mod( T value, T modNum ) {
  while( value < 0 ) {
    value += modNum;
  }
  while( value >= modNum ) {
    value -= modNum;
  }
  return value;
}

}  // namespace Utils
}  // namespace Own2dEngine
}  // namespace SFG

#endif /* UTILS_NUMOPS_H_ */
