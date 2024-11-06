#ifndef LOGGER_SCOPEDLOGGER_H_
#define LOGGER_SCOPEDLOGGER_H_

#include <string>

#include "SFG/Own2dEngine/Logger/_spdlog.h"

namespace SFG {
namespace Own2dEngine {
namespace Logger {

class ScopedLogger {
  public:
  ScopedLogger( spdlogger const& logger, std::string const& startMessage, std::string const& stopMessage );
  ~ScopedLogger();

  private:
  spdlogger logger_;
  std::string startMessage_;
  std::string stopMessage_;
};

}  // namespace Logger
}  // namespace Own2dEngine
}  // namespace SFG

#endif /* LOGGER_SCOPEDLOGGER_H_ */
