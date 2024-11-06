#include "SFG/Own2dEngine/Configuration/configuration.h"

namespace SFG {
namespace Own2dEngine {
namespace Configuration {

Configuration::Configuration( std::string const& configFilepath ) : iniFile_() {
  iniFile_.load( configFilepath.c_str() );
}

}  // namespace Configuration
}  // namespace Own2dEngine
}  // namespace SFG
