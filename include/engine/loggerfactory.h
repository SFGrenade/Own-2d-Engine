#ifndef _ENGINE_LOGGERFACTORY_H_
#define _ENGINE_LOGGERFACTORY_H_

#include <map>
#include <mutex>
#include <string>

#include "_globals/spdlogInclude.h"


namespace SFG {
namespace Engine {

class LoggerFactory {
  public:
  static void init();
  static void deinit();

  static spdlogger get_logger( std::string const& name );

  private:
  static std::shared_ptr< spdlog::sinks::stdout_color_sink_mt > consoleSink_;
  static std::shared_ptr< spdlog::sinks::basic_file_sink_mt > fileSink_;
  static std::string loggerPattern_;
  static std::map< std::string, spdlogger > loggers_;
  static std::mutex loggersMutex_;
};

}  // namespace Engine
}  // namespace SFG

#endif /* _ENGINE_LOGGERFACTORY_H_ */
