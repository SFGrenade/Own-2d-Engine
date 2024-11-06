#ifndef UTILS_CONTAINS_H_
#define UTILS_CONTAINS_H_

#include <algorithm>

namespace SFG {
namespace Own2dEngine {
namespace Utils {

template < std::ranges::common_range C, typename T >
requires std::same_as< std::ranges::range_value_t< C >, T > bool contains( const C& container, const T& element ) {
  return std::find( container.begin(), container.end(), element ) != container.end();
}

}  // namespace Utils
}  // namespace Own2dEngine
}  // namespace SFG

#endif /* UTILS_CONTAINS_H_ */
