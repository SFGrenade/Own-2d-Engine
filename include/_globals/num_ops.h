#ifndef _GLOBALS_NUM_OPS_H_
#define _GLOBALS_NUM_OPS_H_

template < typename T >
T max( T num, T max ) {
  return ( num > max ) ? max : num;
}

template < typename T >
T min( T num, T min ) {
  return ( num < min ) ? min : num;
}

template < typename T >
T clamp( T num, T max, T min ) {
  return ( num > max ) ? max : ( ( num < min ) ? min : num );
}

#endif /* _GLOBALS_NUM_OPS_H_ */
