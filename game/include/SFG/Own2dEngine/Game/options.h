#ifndef ENGINE_OPTIONS_H_
#define ENGINE_OPTIONS_H_

#include <SFG/Own2dEngine/Game/ui/label.h>
#include <SFG/Own2dEngine/Logger/_include.h>
#include <portaudio.h>

namespace SFG {
namespace Own2dEngine {
namespace Game {

class Options {
  public:
  bool GetDebug();
  void SetDebug( bool value );

  void AudioSelectPrevInput();
  void AudioSelectNextInput();
  void AudioSelectPrevOutput();
  void AudioSelectNextOutput();
  void SetAudioDisplayLabels( SFG::Own2dEngine::Game::UI::Label* input, SFG::Own2dEngine::Game::UI::Label* output );
  void SetAudioDevices( std::pair< std::vector< PaDeviceIndex >, std::vector< PaDeviceIndex > > const& devices );
  void SetAudioIndexes( int32_t inputIndex, int32_t outputIndex );

  protected:
  // some functions maybe

  protected:
  int32_t audioInputIndex_ = -1;
  int32_t audioOutputIndex_ = -1;

  private:
  SFG::Own2dEngine::Logger::spdlogger logger_ = SFG::Own2dEngine::Logger::LoggerFactory::get_logger( "Options" );

  bool debug_ = false;

#pragma region audio options helper variables
  SFG::Own2dEngine::Game::UI::Label* audioInputLabel_ = nullptr;
  SFG::Own2dEngine::Game::UI::Label* audioOutputLabel_ = nullptr;
  std::pair< std::vector< PaDeviceIndex >, std::vector< PaDeviceIndex > > devices_;
#pragma endregion
};

}  // namespace Game
}  // namespace Own2dEngine
}  // namespace SFG

#endif /* ENGINE_OPTIONS_H_ */
