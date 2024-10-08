#ifndef _GLOBALS_SPDLOGINCLUDE_H_
#define _GLOBALS_SPDLOGINCLUDE_H_

#include <memory>
#include <string>

// Including FMT headers
#include <fmt/base.h>
#include <fmt/chrono.h>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fmt/std.h>

// Including SPDLOG headers
#ifndef SPDLOG_FMT_EXTERNAL
#define SPDLOG_FMT_EXTERNAL
#endif
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

typedef std::shared_ptr< spdlog::logger > spdlogger;

class ScopedLog {
  public:
  ScopedLog( spdlogger logger, std::string const& startMessage, std::string const& stopMessage );
  ~ScopedLog();

  private:
  spdlogger logger_;
  std::string startMessage_;
  std::string stopMessage_;
};

#endif /* _GLOBALS_SPDLOGINCLUDE_H_ */
