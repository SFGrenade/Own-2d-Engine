#include "engine/loggerfactory.h"


std::shared_ptr< spdlog::sinks::stdout_color_sink_mt > SFG::Engine::LoggerFactory::consoleSink_ = nullptr;
std::shared_ptr< spdlog::sinks::basic_file_sink_mt > SFG::Engine::LoggerFactory::fileSink_ = nullptr;
std::string SFG::Engine::LoggerFactory::loggerPattern_ = "[%Y-%m-%d %H:%M:%S.%e] [thread %t] [%n] [%l] %v";
std::map< std::string, spdlogger > SFG::Engine::LoggerFactory::loggers_ = std::map< std::string, spdlogger >();
std::mutex SFG::Engine::LoggerFactory::loggersMutex_ = std::mutex();

void SFG::Engine::LoggerFactory::init() {
  SFG::Engine::LoggerFactory::loggersMutex_.lock();
  SFG::Engine::LoggerFactory::loggers_.clear();

  SFG::Engine::LoggerFactory::consoleSink_ = std::make_shared< spdlog::sinks::stdout_color_sink_mt >();
  SFG::Engine::LoggerFactory::consoleSink_->set_level( spdlog::level::warn );
  SFG::Engine::LoggerFactory::fileSink_ = std::make_shared< spdlog::sinks::basic_file_sink_mt >( "log.log", true );
  SFG::Engine::LoggerFactory::fileSink_->set_level( spdlog::level::trace );

  spdlog::sinks_init_list truncatedSinkList = { SFG::Engine::LoggerFactory::fileSink_, SFG::Engine::LoggerFactory::consoleSink_ };
  spdlogger mainLogger = std::make_shared< spdlog::logger >( "main", truncatedSinkList.begin(), truncatedSinkList.end() );
  mainLogger->set_level( spdlog::level::trace );
  mainLogger->flush_on( spdlog::level::trace );
  mainLogger->set_pattern( SFG::Engine::LoggerFactory::loggerPattern_ );
  spdlog::register_logger( mainLogger );
  spdlog::set_default_logger( mainLogger );

  SFG::Engine::LoggerFactory::loggers_.insert_or_assign( "main", mainLogger );

  SFG::Engine::LoggerFactory::loggersMutex_.unlock();
}

void SFG::Engine::LoggerFactory::deinit() {
  SFG::Engine::LoggerFactory::loggersMutex_.lock();
  SFG::Engine::LoggerFactory::loggers_.clear();
  SFG::Engine::LoggerFactory::loggersMutex_.unlock();
  spdlog::shutdown();
}

spdlogger SFG::Engine::LoggerFactory::get_logger( std::string const& name ) {
  spdlogger ret = nullptr;

  SFG::Engine::LoggerFactory::loggersMutex_.lock();

  if( SFG::Engine::LoggerFactory::loggers_.find( name ) != SFG::Engine::LoggerFactory::loggers_.end() ) {
    ret = SFG::Engine::LoggerFactory::loggers_.at( name );
  } else {
    spdlog::sinks_init_list truncatedSinkList = { SFG::Engine::LoggerFactory::fileSink_, SFG::Engine::LoggerFactory::consoleSink_ };
    ret = std::make_shared< spdlog::logger >( name, truncatedSinkList.begin(), truncatedSinkList.end() );
    ret->set_level( spdlog::level::trace );
    ret->flush_on( spdlog::level::trace );
    ret->set_pattern( SFG::Engine::LoggerFactory::loggerPattern_ );
    spdlog::register_logger( ret );

    SFG::Engine::LoggerFactory::loggers_.insert_or_assign( name, ret );
  }

  SFG::Engine::LoggerFactory::loggersMutex_.unlock();
  return ret;
}
