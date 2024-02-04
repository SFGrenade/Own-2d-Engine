#ifndef _GLOBALS_LIST_CONTAINS_H_
#define _GLOBALS_LIST_CONTAINS_H_

#include <list>
#include <vector>


template < typename T >
bool list_contains( std::list< T > const& lst, T const& elem ) {
  return std::find( lst.begin(), lst.end(), elem ) != lst.end();
}

template < typename T >
bool vec_contains( std::vector< T > const& vec, T const& elem ) {
  return std::find( vec.begin(), vec.end(), elem ) != vec.end();
}

#endif /* _GLOBALS_LIST_CONTAINS_H_ */
