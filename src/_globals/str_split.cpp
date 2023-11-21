#include "_globals/str_split.h"

std::vector< std::string > str_split( std::string s, std::string delim ) {
  std::vector< std::string > ret;
  size_t pos = 0;
  std::string token;
  while( ( pos = s.find( delim ) ) != std::string::npos ) {
    token = s.substr( 0, pos );
    ret.push_back( token );
    s.erase( 0, pos + delim.length() );
  }
  ret.push_back( s );
  return ret;
}
