#ifndef _GLOBALS_NUM_OPS_H_
#define _GLOBALS_NUM_OPS_H_

template < typename T >
T max( T a, T b ) {
  return ( a < b ) ? b : a;
}

template < typename T >
T min( T a, T b ) {
  return ( a < b ) ? a : b;
}

template < typename T >
T clamp( T num, T max, T min ) {
  return ( num > max ) ? max : ( ( num < min ) ? min : num );
}

#endif /* _GLOBALS_NUM_OPS_H_ */
