#ifndef LOGGER_LOGGERFACTORY_H_
#define LOGGER_LOGGERFACTORY_H_

#include <map>
#include <memory>
#include <mutex>
#include <string>

#include "SFG/Own2dEngine/Logger/_spdlog.h"

namespace SFG {
namespace Own2dEngine {
namespace Logger {

class LoggerFactory {
  public:
  static void init( std::string const& logFileName = "log.log", bool printOnStdOut = true );
  static void deinit();

  static spdlogger get_logger( std::string const& name );

  private:
  static std::shared_ptr< spdlog::sinks::stdout_color_sink_mt > consoleSink_;
  static std::shared_ptr< spdlog::sinks::basic_file_sink_mt > fileSink_;
  static std::string loggerPattern_;
  static std::map< std::string, spdlogger > loggers_;
  static std::mutex loggersMutex_;
};

}  // namespace Logger
}  // namespace Own2dEngine
}  // namespace SFG

#endif /* LOGGER_LOGGERFACTORY_H_ */
