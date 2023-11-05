#include "confighandler/confighandler.h"

namespace SFG {
spdlogger ConfigHandler::logger = nullptr;

spdlog::level::level_enum ConfigHandler::config_Logging_StdOutLevel;
spdlog::level::level_enum ConfigHandler::config_Logging_LogFileLevel;
uint16_t ConfigHandler::config_Font_PixelSize;
uint16_t ConfigHandler::config_Input_StopGameKey;
double ConfigHandler::config_Logic_LogicInterval;
double ConfigHandler::config_Logic_NetworkInterval;
double ConfigHandler::config_Logic_PerformanceInterval;
std::string ConfigHandler::config_Network_Host;
uint16_t ConfigHandler::config_Network_PortReqRep;
uint16_t ConfigHandler::config_Network_PortPubSub;
uint16_t ConfigHandler::config_Window_Width;
uint16_t ConfigHandler::config_Window_Height;
uint32_t ConfigHandler::config_Audio_SampleRate;
uint32_t ConfigHandler::config_Audio_Format;
uint16_t ConfigHandler::config_Audio_Channels;
uint32_t ConfigHandler::config_Audio_ChunkSize;
std::string ConfigHandler::config_Rendering_Renderer;
std::string ConfigHandler::config_Rendering_PerformanceStringTextColor;
std::string ConfigHandler::config_Rendering_PerformanceStringBackgroundColor;

void ConfigHandler::InitializeNoLog() {
  std::string configPath = R"(./config/engine.ini)";

  CSimpleIniA ini;
  ini.SetUnicode();

  SI_Error rc;
  rc = ini.LoadFile( configPath.c_str() );
  if( rc != SI_OK ) {
    throw new std::runtime_error( "Error reading config file at \"" + configPath + "\"" );
  }

  ConfigHandler::config_Logging_StdOutLevel = static_cast< spdlog::level::level_enum >( ini.GetLongValue( "Logging", "StdOutLevel", static_cast< long >( spdlog::level::warn ) ) );
  ConfigHandler::config_Logging_LogFileLevel = static_cast< spdlog::level::level_enum >( ini.GetLongValue( "Logging", "LogFileLevel", static_cast< long >( spdlog::level::trace ) ) );
  ConfigHandler::config_Font_PixelSize = static_cast< uint16_t >( ini.GetLongValue( "Font", "PixelSize", 18 ) );
  ConfigHandler::config_Input_StopGameKey = static_cast< uint16_t >( ini.GetLongValue( "Input", "StopGameKey", SDLK_ESCAPE ) );
  ConfigHandler::config_Logic_LogicInterval = static_cast< double >( ini.GetDoubleValue( "Logic", "LogicInterval", 50.0 ) );
  ConfigHandler::config_Logic_NetworkInterval = static_cast< double >( ini.GetDoubleValue( "Logic", "NetworkInterval", 50.0 ) );
  ConfigHandler::config_Logic_PerformanceInterval = static_cast< double >( ini.GetDoubleValue( "Logic", "PerformanceInterval", 1.0 ) );
  ConfigHandler::config_Network_Host = ini.GetValue( "Network", "Host", "127.0.0.1" );
  ConfigHandler::config_Network_PortReqRep = static_cast< uint16_t >( ini.GetLongValue( "Network", "PortReqRep", 13337 ) );
  ConfigHandler::config_Network_PortPubSub = static_cast< uint16_t >( ini.GetLongValue( "Network", "PortPubSub", 13338 ) );
  ConfigHandler::config_Window_Width = static_cast< uint16_t >( ini.GetLongValue( "Window", "Width", 1600 ) );
  ConfigHandler::config_Window_Height = static_cast< uint16_t >( ini.GetLongValue( "Window", "Height", 900 ) );
  ConfigHandler::config_Audio_SampleRate = static_cast< uint32_t >( ini.GetLongValue( "Audio", "SampleRate", 44100 ) );
  ConfigHandler::config_Audio_Format = static_cast< uint32_t >( ini.GetLongValue( "Audio", "Format", MIX_DEFAULT_FORMAT ) );
  ConfigHandler::config_Audio_Channels = static_cast< uint16_t >( ini.GetLongValue( "Audio", "Channels", 2 ) );
  ConfigHandler::config_Audio_ChunkSize = static_cast< uint16_t >( ini.GetLongValue( "Audio", "ChunkSize", 2048 ) );
  ConfigHandler::config_Rendering_Renderer = ini.GetValue( "Rendering", "Renderer", "" );
  ConfigHandler::config_Rendering_PerformanceStringTextColor = ini.GetValue( "Rendering", "PerformanceStringTextColor", "255 255 255 255" );
  ConfigHandler::config_Rendering_PerformanceStringBackgroundColor = ini.GetValue( "Rendering", "PerformanceStringBackgroundColor", "0 0 0 0" );

  ini.SetLongValue( "Logging", "StdOutLevel", static_cast< long >( ConfigHandler::config_Logging_StdOutLevel ), R"(either of trace = 0, debug = 1, info = 2, warn = 3, err = 4, critical = 5, off = 6)" );
  ini.SetLongValue( "Logging", "LogFileLevel", static_cast< long >( ConfigHandler::config_Logging_LogFileLevel ), R"(either of trace = 0, debug = 1, info = 2, warn = 3, err = 4, critical = 5, off = 6)" );
  ini.SetLongValue( "Font", "PixelSize", static_cast< long >( ConfigHandler::config_Font_PixelSize ), R"(Size of the displayed text, in pixels)" );
  ini.SetLongValue( "Input", "StopGameKey", static_cast< long >( ConfigHandler::config_Input_StopGameKey ), R"(See https://wiki.libsdl.org/SDL2/SDLKeycodeLookup for the number)" );
  ini.SetDoubleValue( "Logic", "LogicInterval", static_cast< double >( ConfigHandler::config_Logic_LogicInterval ), R"(How many times per second will the LogicUpdate be called?)" );
  ini.SetDoubleValue( "Logic", "NetworkInterval", static_cast< double >( ConfigHandler::config_Logic_NetworkInterval ), R"(How many times per second will the network loop be run?)" );
  ini.SetDoubleValue( "Logic", "PerformanceInterval", static_cast< double >( ConfigHandler::config_Logic_PerformanceInterval ), R"(How many times per second will the performance be evaluated?)" );
  ini.SetValue( "Network", "Host", ConfigHandler::config_Network_Host.c_str(), R"(Server address)" );
  ini.SetLongValue( "Network", "PortReqRep", static_cast< long >( ConfigHandler::config_Network_PortReqRep ), R"(Port of the request-reply service)" );
  ini.SetLongValue( "Network", "PortPubSub", static_cast< long >( ConfigHandler::config_Network_PortPubSub ), R"(Port of the publish-subscribe service)" );
  ini.SetLongValue( "Window", "Width", static_cast< long >( ConfigHandler::config_Window_Width ), R"(Width of the window, in pixels)" );
  ini.SetLongValue( "Window", "Height", static_cast< long >( ConfigHandler::config_Window_Height ), R"(Height of the window, in pixels)" );
  ini.SetLongValue( "Audio", "SampleRate", static_cast< long >( ConfigHandler::config_Audio_SampleRate ), R"(Like the sample rate of audio files)" );
  ini.SetLongValue( "Audio", "Format", static_cast< long >( ConfigHandler::config_Audio_Format ), R"(SDL_mixer.h - `MIX_DEFAULT_FORMAT` - defaults to `AUDIO_S16LSB`, which is 0x8010)", true );
  ini.SetLongValue( "Audio", "Channels", static_cast< long >( ConfigHandler::config_Audio_Channels ), R"(1 = mono, 2 = stereo, ...)" );
  ini.SetLongValue( "Audio", "ChunkSize", static_cast< long >( ConfigHandler::config_Audio_ChunkSize ), R"(Audio buffer size in sample frames (samples divided by channel count), 2048 is a good default)" );
  ini.SetValue( "Rendering", "Renderer", ConfigHandler::config_Rendering_Renderer.c_str(), R"(nothing for the first available or check log file for a list of available renderers)" );
  ini.SetValue( "Rendering", "PerformanceStringTextColor", ConfigHandler::config_Rendering_PerformanceStringTextColor.c_str(), R"(Red Green Blue Alpha (each 0 - 255))" );
  ini.SetValue( "Rendering", "PerformanceStringBackgroundColor", ConfigHandler::config_Rendering_PerformanceStringBackgroundColor.c_str(), R"(Red Green Blue Alpha (each 0 - 255))" );

  rc = ini.SaveFile( configPath.c_str() );
  if( rc != SI_OK ) {
    throw new std::runtime_error( "Error saving config file at \"" + configPath + "\"" );
  }
}

void ConfigHandler::Initialize() {
  ConfigHandler::logger = spdlog::get( "ConfigHandler" );
  ConfigHandler::logger->trace( "Initialize()" );

  ConfigHandler::logger->debug( "Initialize - Loaded values:" );
  ConfigHandler::logger->debug( "Initialize -     config_Logging_StdOutLevel = {:d}", static_cast< uint32_t >( config_Logging_StdOutLevel ) );
  ConfigHandler::logger->debug( "Initialize -     config_Logging_LogFileLevel = {:d}", static_cast< uint32_t >( config_Logging_LogFileLevel ) );
  ConfigHandler::logger->debug( "Initialize -     config_Font_PixelSize = {:d}", ConfigHandler::config_Font_PixelSize );
  ConfigHandler::logger->debug( "Initialize -     config_Input_StopGameKey = {:d}", ConfigHandler::config_Input_StopGameKey );
  ConfigHandler::logger->debug( "Initialize -     config_Logic_LogicInterval = {:f}", ConfigHandler::config_Logic_LogicInterval );
  ConfigHandler::logger->debug( "Initialize -     config_Logic_NetworkInterval = {:f}", ConfigHandler::config_Logic_NetworkInterval );
  ConfigHandler::logger->debug( "Initialize -     config_Logic_PerformanceInterval = {:f}", ConfigHandler::config_Logic_PerformanceInterval );
  ConfigHandler::logger->debug( "Initialize -     config_Network_Host = \"{:s}\"", ConfigHandler::config_Network_Host );
  ConfigHandler::logger->debug( "Initialize -     config_Network_PortReqRep = {:d}", ConfigHandler::config_Network_PortReqRep );
  ConfigHandler::logger->debug( "Initialize -     config_Network_PortPubSub = {:d}", ConfigHandler::config_Network_PortPubSub );
  ConfigHandler::logger->debug( "Initialize -     config_Window_Width = {:d}", ConfigHandler::config_Window_Width );
  ConfigHandler::logger->debug( "Initialize -     config_Window_Height = {:d}", ConfigHandler::config_Window_Height );
  ConfigHandler::logger->debug( "Initialize -     config_Audio_SampleRate = {:d}", ConfigHandler::config_Audio_SampleRate );
  ConfigHandler::logger->debug( "Initialize -     config_Audio_Format = {:d}", ConfigHandler::config_Audio_Format );
  ConfigHandler::logger->debug( "Initialize -     config_Audio_Channels = {:d}", ConfigHandler::config_Audio_Channels );
  ConfigHandler::logger->debug( "Initialize -     config_Audio_ChunkSize = {:d}", ConfigHandler::config_Audio_ChunkSize );
  ConfigHandler::logger->debug( "Initialize -     config_Rendering_Renderer = \"{:s}\"", ConfigHandler::config_Rendering_Renderer );
  ConfigHandler::logger->debug( "Initialize -     config_Rendering_PerformanceStringTextColor = \"{:s}\"", ConfigHandler::config_Rendering_PerformanceStringTextColor );
  ConfigHandler::logger->debug( "Initialize -     config_Rendering_PerformanceStringBackgroundColor = \"{:s}\"", ConfigHandler::config_Rendering_PerformanceStringBackgroundColor );

  ConfigHandler::logger->trace( "Initialize()~" );
}

void ConfigHandler::Destroy() {
  ConfigHandler::logger->trace( "Destroy()" );

  ConfigHandler::logger->trace( "Destroy()~" );
}

spdlog::level::level_enum ConfigHandler::get_Logging_StdOutLevel() {
  return config_Logging_StdOutLevel;
}

spdlog::level::level_enum ConfigHandler::get_Logging_LogFileLevel() {
  return config_Logging_LogFileLevel;
}

uint16_t ConfigHandler::get_Font_PixelSize() {
  return config_Font_PixelSize;
}

uint16_t ConfigHandler::get_Input_StopGameKey() {
  return config_Input_StopGameKey;
}

double ConfigHandler::get_Logic_LogicInterval() {
  return config_Logic_LogicInterval;
}

double ConfigHandler::get_Logic_NetworkInterval() {
  return config_Logic_NetworkInterval;
}

double ConfigHandler::get_Logic_PerformanceInterval() {
  return config_Logic_PerformanceInterval;
}

std::string ConfigHandler::get_Network_Host() {
  return config_Network_Host;
}

uint16_t ConfigHandler::get_Network_PortReqRep() {
  return config_Network_PortReqRep;
}

uint16_t ConfigHandler::get_Network_PortPubSub() {
  return config_Network_PortPubSub;
}

uint16_t ConfigHandler::get_Window_Width() {
  return config_Window_Width;
}

uint16_t ConfigHandler::get_Window_Height() {
  return config_Window_Height;
}

uint32_t ConfigHandler::get_Audio_SampleRate() {
  return config_Audio_SampleRate;
}

uint32_t ConfigHandler::get_Audio_Format() {
  return config_Audio_Format;
}

uint16_t ConfigHandler::get_Audio_Channels() {
  return config_Audio_Channels;
}

uint32_t ConfigHandler::get_Audio_ChunkSize() {
  return config_Audio_ChunkSize;
}

std::string ConfigHandler::get_Rendering_Renderer() {
  return config_Rendering_Renderer;
}

std::string ConfigHandler::get_Rendering_PerformanceStringTextColor() {
  return config_Rendering_PerformanceStringTextColor;
}

std::string ConfigHandler::get_Rendering_PerformanceStringBackgroundColor() {
  return config_Rendering_PerformanceStringBackgroundColor;
}
}  // namespace SFG
