#pragma once

#include <cstdint>
#include <string>

// Including our headers
#include "_globals/sdlInclude.h"
#include "_globals/spdlogInclude.h"

namespace SFG {
class ConfigHandler {
  private:
  static spdlogger logger;

  static spdlog::level::level_enum config_Logging_StdOutLevel;
  static spdlog::level::level_enum config_Logging_LogFileLevel;
  static int config_Font_PixelSize;
  static SDL_Keycode config_Input_StopGameKey;
  static double config_Logic_LogicInterval;
  static double config_Logic_PerformanceInterval;
  static std::string config_Network_Host;
  static uint16_t config_Network_PortReqRep;
  static uint16_t config_Network_PortPubSub;
  static int config_Window_Width;
  static int config_Window_Height;
  static uint32_t config_Audio_SampleRate;
  static uint16_t config_Audio_Format;
  static uint32_t config_Audio_Channels;
  static uint32_t config_Audio_ChunkSize;
  static std::string config_Rendering_Renderer;
  static std::string config_Rendering_PerformanceStringTextColor;
  static std::string config_Rendering_PerformanceStringBackgroundColor;

  public:
  static void InitializeNoLog();
  static void Initialize();
  static void Destroy();

  static spdlog::level::level_enum get_Logging_StdOutLevel();
  static spdlog::level::level_enum get_Logging_LogFileLevel();
  static int get_Font_PixelSize();
  static SDL_Keycode get_Input_StopGameKey();
  static double get_Logic_LogicInterval();
  static double get_Logic_PerformanceInterval();
  static std::string get_Network_Host();
  static uint16_t get_Network_PortReqRep();
  static uint16_t get_Network_PortPubSub();
  static int get_Window_Width();
  static int get_Window_Height();
  static uint32_t get_Audio_SampleRate();
  static uint16_t get_Audio_Format();
  static uint32_t get_Audio_Channels();
  static uint32_t get_Audio_ChunkSize();
  static std::string get_Rendering_Renderer();
  static std::string get_Rendering_PerformanceStringTextColor();
  static std::string get_Rendering_PerformanceStringBackgroundColor();
};
}  // namespace SFG
