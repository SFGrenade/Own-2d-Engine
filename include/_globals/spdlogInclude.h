#ifndef _GLOBALS_SPDLOGINCLUDE_H_
#define _GLOBALS_SPDLOGINCLUDE_H_

// Including FMT headers
#include <fmt/chrono.h>
#include <fmt/core.h>

// Including SPDLOG headers
#ifndef SPDLOG_FMT_EXTERNAL
#define SPDLOG_FMT_EXTERNAL
#endif
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

typedef std::shared_ptr< spdlog::logger > spdlogger;

#endif /* _GLOBALS_SPDLOGINCLUDE_H_ */
