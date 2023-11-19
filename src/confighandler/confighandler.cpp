#include "confighandler/confighandler.h"

#include <SimpleIni.h>

#include "_globals/sdlInclude.h"

namespace SFG {
spdlogger ConfigHandler::logger_ = nullptr;

spdlog::level::level_enum ConfigHandler::config_Logging_StdOutLevel_;
spdlog::level::level_enum ConfigHandler::config_Logging_LogFileLevel_;
int ConfigHandler::config_Font_PixelSize_;
SDL_Keycode ConfigHandler::config_Input_StopGameKey_;
double ConfigHandler::config_Logic_LogicInterval_;
double ConfigHandler::config_Logic_PerformanceInterval_;
std::string ConfigHandler::config_Network_Host_;
uint16_t ConfigHandler::config_Network_PortReqRep_;
uint16_t ConfigHandler::config_Network_PortPubSub_;
int ConfigHandler::config_Window_Width_;
int ConfigHandler::config_Window_Height_;
uint32_t ConfigHandler::config_Audio_SampleRate_;
uint16_t ConfigHandler::config_Audio_Format_;
uint32_t ConfigHandler::config_Audio_Channels_;
uint32_t ConfigHandler::config_Audio_ChunkSize_;
std::string ConfigHandler::config_Rendering_Renderer_;
std::string ConfigHandler::config_Rendering_PerformanceStringTextColor_;
std::string ConfigHandler::config_Rendering_PerformanceStringBackgroundColor_;

void ConfigHandler::InitializeNoLog() {
  std::string configPath = R"(./config/engine.ini)";

  CSimpleIniA ini;
  ini.SetUnicode();

  SI_Error rc;
  rc = ini.LoadFile( configPath.c_str() );
  if( rc != SI_OK ) {
    throw new std::runtime_error( "Error reading config file at \"" + configPath + "\"" );
  }

  ConfigHandler::config_Logging_StdOutLevel_
      = static_cast< spdlog::level::level_enum >( ini.GetLongValue( "Logging", "StdOutLevel", static_cast< int >( spdlog::level::warn ) ) );
  ConfigHandler::config_Logging_LogFileLevel_
      = static_cast< spdlog::level::level_enum >( ini.GetLongValue( "Logging", "LogFileLevel", static_cast< int >( spdlog::level::trace ) ) );
  ConfigHandler::config_Font_PixelSize_ = static_cast< int >( ini.GetLongValue( "Font", "PixelSize", 18 ) );
  ConfigHandler::config_Input_StopGameKey_ = static_cast< SDL_Keycode >( ini.GetLongValue( "Input", "StopGameKey", SDLK_ESCAPE ) );
  ConfigHandler::config_Logic_LogicInterval_ = static_cast< double >( ini.GetDoubleValue( "Logic", "LogicInterval", 50.0 ) );
  ConfigHandler::config_Logic_PerformanceInterval_ = static_cast< double >( ini.GetDoubleValue( "Logic", "PerformanceInterval", 1.0 ) );
  ConfigHandler::config_Network_Host_ = ini.GetValue( "Network", "Host", "127.0.0.1" );
  ConfigHandler::config_Network_PortReqRep_ = static_cast< uint16_t >( ini.GetLongValue( "Network", "PortReqRep", 13337 ) );
  ConfigHandler::config_Network_PortPubSub_ = static_cast< uint16_t >( ini.GetLongValue( "Network", "PortPubSub", 13338 ) );
  ConfigHandler::config_Window_Width_ = static_cast< int >( ini.GetLongValue( "Window", "Width", 800 ) );
  ConfigHandler::config_Window_Height_ = static_cast< int >( ini.GetLongValue( "Window", "Height", 600 ) );
  ConfigHandler::config_Audio_SampleRate_ = static_cast< uint32_t >( ini.GetLongValue( "Audio", "SampleRate", 44100 ) );
  ConfigHandler::config_Audio_Format_ = static_cast< uint16_t >( ini.GetLongValue( "Audio", "Format", MIX_DEFAULT_FORMAT ) );
  ConfigHandler::config_Audio_Channels_ = static_cast< uint32_t >( ini.GetLongValue( "Audio", "Channels", 2 ) );
  ConfigHandler::config_Audio_ChunkSize_ = static_cast< uint32_t >( ini.GetLongValue( "Audio", "ChunkSize", 2048 ) );
  ConfigHandler::config_Rendering_Renderer_ = ini.GetValue( "Rendering", "Renderer", "" );
  ConfigHandler::config_Rendering_PerformanceStringTextColor_ = ini.GetValue( "Rendering", "PerformanceStringTextColor", "255 255 255 255" );
  ConfigHandler::config_Rendering_PerformanceStringBackgroundColor_ = ini.GetValue( "Rendering", "PerformanceStringBackgroundColor", "0 0 0 0" );

  ini.SetLongValue( "Logging",
                    "StdOutLevel",
                    static_cast< long >( ConfigHandler::config_Logging_StdOutLevel_ ),
                    R"(either of trace = 0, debug = 1, info = 2, warn = 3, err = 4, critical = 5, off = 6)" );
  ini.SetLongValue( "Logging",
                    "LogFileLevel",
                    static_cast< long >( ConfigHandler::config_Logging_LogFileLevel_ ),
                    R"(either of trace = 0, debug = 1, info = 2, warn = 3, err = 4, critical = 5, off = 6)" );
  ini.SetLongValue( "Font", "PixelSize", static_cast< long >( ConfigHandler::config_Font_PixelSize_ ), R"(Size of the displayed text, in pixels)" );
  ini.SetLongValue( "Input",
                    "StopGameKey",
                    static_cast< long >( ConfigHandler::config_Input_StopGameKey_ ),
                    R"(See https://wiki.libsdl.org/SDL2/SDLKeycodeLookup for the number)" );
  ini.SetDoubleValue( "Logic",
                      "LogicInterval",
                      static_cast< double >( ConfigHandler::config_Logic_LogicInterval_ ),
                      R"(How many times per second will the LogicUpdate be called?)" );
  ini.SetDoubleValue( "Logic",
                      "PerformanceInterval",
                      static_cast< double >( ConfigHandler::config_Logic_PerformanceInterval_ ),
                      R"(How many times per second will the performance be evaluated?)" );
  ini.SetValue( "Network", "Host", ConfigHandler::config_Network_Host_.c_str(), R"(Server address)" );
  ini.SetLongValue( "Network", "PortReqRep", static_cast< long >( ConfigHandler::config_Network_PortReqRep_ ), R"(Port of the request-reply service)" );
  ini.SetLongValue( "Network", "PortPubSub", static_cast< long >( ConfigHandler::config_Network_PortPubSub_ ), R"(Port of the publish-subscribe service)" );
  ini.SetLongValue( "Window", "Width", static_cast< long >( ConfigHandler::config_Window_Width_ ), R"(Width of the window, in pixels)" );
  ini.SetLongValue( "Window", "Height", static_cast< long >( ConfigHandler::config_Window_Height_ ), R"(Height of the window, in pixels)" );
  ini.SetLongValue( "Audio", "SampleRate", static_cast< long >( ConfigHandler::config_Audio_SampleRate_ ), R"(Like the sample rate of audio files)" );
  ini.SetLongValue( "Audio",
                    "Format",
                    static_cast< long >( ConfigHandler::config_Audio_Format_ ),
                    R"(SDL_mixer.h - `MIX_DEFAULT_FORMAT` - defaults to `AUDIO_S16LSB`, which is 0x8010)",
                    true );
  ini.SetLongValue( "Audio", "Channels", static_cast< long >( ConfigHandler::config_Audio_Channels_ ), R"(1 = mono, 2 = stereo, ...)" );
  ini.SetLongValue( "Audio",
                    "ChunkSize",
                    static_cast< long >( ConfigHandler::config_Audio_ChunkSize_ ),
                    R"(Audio buffer size in sample frames (samples divided by channel count), 2048 is a good default)" );
  ini.SetValue( "Rendering",
                "Renderer",
                ConfigHandler::config_Rendering_Renderer_.c_str(),
                R"(nothing for the first available or check log file for a list of available renderers)" );
  ini.SetValue( "Rendering",
                "PerformanceStringTextColor",
                ConfigHandler::config_Rendering_PerformanceStringTextColor_.c_str(),
                R"(Red Green Blue Alpha (each 0 - 255))" );
  ini.SetValue( "Rendering",
                "PerformanceStringBackgroundColor",
                ConfigHandler::config_Rendering_PerformanceStringBackgroundColor_.c_str(),
                R"(Red Green Blue Alpha (each 0 - 255))" );

  rc = ini.SaveFile( configPath.c_str() );
  if( rc != SI_OK ) {
    throw new std::runtime_error( "Error saving config file at \"" + configPath + "\"" );
  }
}

void ConfigHandler::Initialize() {
  ConfigHandler::logger_ = spdlog::get( "ConfigHandler" );
  ConfigHandler::logger_->trace( fmt::runtime( "Initialize()" ) );

  ConfigHandler::logger_->debug( fmt::runtime( "Initialize - Loaded values:" ) );
  ConfigHandler::logger_->debug( fmt::runtime( "Initialize -     config_Logging_StdOutLevel = {:d}" ),
                                 static_cast< int >( ConfigHandler::config_Logging_StdOutLevel_ ) );
  ConfigHandler::logger_->debug( fmt::runtime( "Initialize -     config_Logging_LogFileLevel = {:d}" ),
                                 static_cast< int >( ConfigHandler::config_Logging_LogFileLevel_ ) );
  ConfigHandler::logger_->debug( fmt::runtime( "Initialize -     config_Font_PixelSize = {:d}" ), ConfigHandler::config_Font_PixelSize_ );
  ConfigHandler::logger_->debug( fmt::runtime( "Initialize -     config_Input_StopGameKey = {:d}" ), ConfigHandler::config_Input_StopGameKey_ );
  ConfigHandler::logger_->debug( fmt::runtime( "Initialize -     config_Logic_LogicInterval = {:f}" ), ConfigHandler::config_Logic_LogicInterval_ );
  ConfigHandler::logger_->debug( fmt::runtime( "Initialize -     config_Logic_PerformanceInterval = {:f}" ), ConfigHandler::config_Logic_PerformanceInterval_ );
  ConfigHandler::logger_->debug( fmt::runtime( "Initialize -     config_Network_Host = \"{:s}\"" ), ConfigHandler::config_Network_Host_ );
  ConfigHandler::logger_->debug( fmt::runtime( "Initialize -     config_Network_PortReqRep = {:d}" ), ConfigHandler::config_Network_PortReqRep_ );
  ConfigHandler::logger_->debug( fmt::runtime( "Initialize -     config_Network_PortPubSub = {:d}" ), ConfigHandler::config_Network_PortPubSub_ );
  ConfigHandler::logger_->debug( fmt::runtime( "Initialize -     config_Window_Width = {:d}" ), ConfigHandler::config_Window_Width_ );
  ConfigHandler::logger_->debug( fmt::runtime( "Initialize -     config_Window_Height = {:d}" ), ConfigHandler::config_Window_Height_ );
  ConfigHandler::logger_->debug( fmt::runtime( "Initialize -     config_Audio_SampleRate = {:d}" ), ConfigHandler::config_Audio_SampleRate_ );
  ConfigHandler::logger_->debug( fmt::runtime( "Initialize -     config_Audio_Format = {:d}" ), ConfigHandler::config_Audio_Format_ );
  ConfigHandler::logger_->debug( fmt::runtime( "Initialize -     config_Audio_Channels = {:d}" ), ConfigHandler::config_Audio_Channels_ );
  ConfigHandler::logger_->debug( fmt::runtime( "Initialize -     config_Audio_ChunkSize = {:d}" ), ConfigHandler::config_Audio_ChunkSize_ );
  ConfigHandler::logger_->debug( fmt::runtime( "Initialize -     config_Rendering_Renderer = \"{:s}\"" ), ConfigHandler::config_Rendering_Renderer_ );
  ConfigHandler::logger_->debug( fmt::runtime( "Initialize -     config_Rendering_PerformanceStringTextColor = \"{:s}\"" ),
                                 ConfigHandler::config_Rendering_PerformanceStringTextColor_ );
  ConfigHandler::logger_->debug( fmt::runtime( "Initialize -     config_Rendering_PerformanceStringBackgroundColor = \"{:s}\"" ),
                                 ConfigHandler::config_Rendering_PerformanceStringBackgroundColor_ );

  ConfigHandler::logger_->trace( fmt::runtime( "Initialize()~" ) );
}

void ConfigHandler::Destroy() {
  ConfigHandler::logger_->trace( fmt::runtime( "Destroy()" ) );

  ConfigHandler::logger_->trace( fmt::runtime( "Destroy()~" ) );
}

spdlog::level::level_enum ConfigHandler::get_Logging_StdOutLevel() {
  return ConfigHandler::config_Logging_StdOutLevel_;
}

spdlog::level::level_enum ConfigHandler::get_Logging_LogFileLevel() {
  return ConfigHandler::config_Logging_LogFileLevel_;
}

int ConfigHandler::get_Font_PixelSize() {
  return ConfigHandler::config_Font_PixelSize_;
}

SDL_Keycode ConfigHandler::get_Input_StopGameKey() {
  return ConfigHandler::config_Input_StopGameKey_;
}

double ConfigHandler::get_Logic_LogicInterval() {
  return ConfigHandler::config_Logic_LogicInterval_;
}

double ConfigHandler::get_Logic_PerformanceInterval() {
  return ConfigHandler::config_Logic_PerformanceInterval_;
}

std::string ConfigHandler::get_Network_Host() {
  return ConfigHandler::config_Network_Host_;
}

uint16_t ConfigHandler::get_Network_PortReqRep() {
  return ConfigHandler::config_Network_PortReqRep_;
}

uint16_t ConfigHandler::get_Network_PortPubSub() {
  return ConfigHandler::config_Network_PortPubSub_;
}

int ConfigHandler::get_Window_Width() {
  return ConfigHandler::config_Window_Width_;
}

int ConfigHandler::get_Window_Height() {
  return ConfigHandler::config_Window_Height_;
}

uint32_t ConfigHandler::get_Audio_SampleRate() {
  return ConfigHandler::config_Audio_SampleRate_;
}

uint16_t ConfigHandler::get_Audio_Format() {
  return ConfigHandler::config_Audio_Format_;
}

uint32_t ConfigHandler::get_Audio_Channels() {
  return ConfigHandler::config_Audio_Channels_;
}

uint32_t ConfigHandler::get_Audio_ChunkSize() {
  return ConfigHandler::config_Audio_ChunkSize_;
}

std::string ConfigHandler::get_Rendering_Renderer() {
  return ConfigHandler::config_Rendering_Renderer_;
}

std::string ConfigHandler::get_Rendering_PerformanceStringTextColor() {
  return ConfigHandler::config_Rendering_PerformanceStringTextColor_;
}

std::string ConfigHandler::get_Rendering_PerformanceStringBackgroundColor() {
  return ConfigHandler::config_Rendering_PerformanceStringBackgroundColor_;
}
}  // namespace SFG
