#include "SFG/Own2dEngine/Game/options.h"

#include <SFG/Own2dEngine/Game/audioManager.h>
#include <SFG/Own2dEngine/Utils/_include.h>

namespace SFG {
namespace Own2dEngine {
namespace Game {

bool Options::GetDebug() {
  return debug_;
}

void Options::SetDebug( bool value ) {
  debug_ = value;
}

void Options::AudioSelectPrevInput() {
  SetAudioIndexes( audioInputIndex_ - 1, audioOutputIndex_ );
}

void Options::AudioSelectNextInput() {
  SetAudioIndexes( audioInputIndex_ + 1, audioOutputIndex_ );
}

void Options::AudioSelectPrevOutput() {
  SetAudioIndexes( audioInputIndex_, audioOutputIndex_ - 1 );
}

void Options::AudioSelectNextOutput() {
  SetAudioIndexes( audioInputIndex_, audioOutputIndex_ + 1 );
}

void Options::SetAudioDisplayLabels( SFG::Own2dEngine::Game::UI::Label* input, SFG::Own2dEngine::Game::UI::Label* output ) {
  audioInputLabel_ = input;
  audioOutputLabel_ = output;
}

void Options::SetAudioDevices( std::pair< std::vector< PaDeviceIndex >, std::vector< PaDeviceIndex > > const& devices ) {
  devices_ = devices;
}

void Options::SetAudioIndexes( int32_t inputIndex, int32_t outputIndex ) {
  spdlog::trace( "[TempAudioStruct] SetAudioDeviceIndexes( inputIndex: {}, outputIndex: {} )", inputIndex, outputIndex );
  inputIndex = SFG::Own2dEngine::Utils::mod< int32_t >( inputIndex + 1, devices_.first.size() + 1 ) - 1;
  outputIndex = SFG::Own2dEngine::Utils::mod< int32_t >( outputIndex + 1, devices_.second.size() + 1 ) - 1;

  PaDeviceIndex tmpInput;
  PaDeviceIndex tmpOutput;
  if( inputIndex < 0 ) {
    tmpInput = Pa_GetDefaultInputDevice();
  } else {
    tmpInput = devices_.first[inputIndex];
  }
  if( outputIndex < 0 ) {
    tmpOutput = Pa_GetDefaultOutputDevice();
  } else {
    tmpOutput = devices_.second[outputIndex];
  }

  {
    PaDeviceInfo const* deviceInfo = nullptr;
    PaHostApiInfo const* hostApiInfo = nullptr;
    std::string deviceName = "__UNKNOWN__";
    std::string apiName = "__UNKNOWN__";

    deviceInfo = Pa_GetDeviceInfo( tmpInput );
    if( deviceInfo ) {
      deviceName = deviceInfo->name;
      hostApiInfo = Pa_GetHostApiInfo( deviceInfo->hostApi );
      if( hostApiInfo ) {
        apiName = hostApiInfo->name;
      }
    }
    if( audioInputLabel_ ) {
      audioInputLabel_->SetText( fmt::format( "Audio Input: ({}) {}", apiName, deviceName ) );
    }

    deviceInfo = nullptr;
    hostApiInfo = nullptr;
    deviceName = "__UNKNOWN__";
    apiName = "__UNKNOWN__";

    deviceInfo = Pa_GetDeviceInfo( tmpOutput );
    if( deviceInfo ) {
      deviceName = deviceInfo->name;
      hostApiInfo = Pa_GetHostApiInfo( deviceInfo->hostApi );
      if( hostApiInfo ) {
        apiName = hostApiInfo->name;
      }
    }
    if( audioOutputLabel_ ) {
      audioOutputLabel_->SetText( fmt::format( "Audio Output: ({}) {}", apiName, deviceName ) );
    }
  }

  audioInputIndex_ = inputIndex;
  audioOutputIndex_ = outputIndex;

  SFG::Own2dEngine::Game::AudioManager::StopAudio( "Background Music" );

  SFG::Own2dEngine::Game::AudioManager::Start( tmpInput, tmpOutput );

  SFG::Own2dEngine::Game::AudioManager::LoadAudioFile( "Background Music",
                                                       SFG::Own2dEngine::Game::AudioManager::AudioType::BGM,
                                                       "Resources/Audio/8Bit 01 w.wav" );
}

}  // namespace Game
}  // namespace Own2dEngine
}  // namespace SFG
