#ifndef CONFIGURATION_CONFIGURATION_H_
#define CONFIGURATION_CONFIGURATION_H_

#include <inicpp.h>
#include <string>

namespace SFG {
namespace Own2dEngine {
namespace Configuration {

class Configuration {
  public:
  Configuration( std::string const& configFilepath );

  template < typename T >
  T get( std::string const& section, std::string const& key ) {
    return iniFile_[section][key].as< T >();
  }

  private:
  ini::IniFile iniFile_;
};

}  // namespace Configuration
}  // namespace Own2dEngine
}  // namespace SFG

#endif /* CONFIGURATION_CONFIGURATION_H_ */
