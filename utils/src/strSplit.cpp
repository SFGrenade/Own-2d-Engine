#include "SFG/Own2dEngine/Utils/strSplit.h"

namespace SFG {
namespace Own2dEngine {
namespace Utils {

std::vector< std::string > strSplit( std::string s, std::string delim ) {
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

}  // namespace Utils
}  // namespace Own2dEngine
}  // namespace SFG
