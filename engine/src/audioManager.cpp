#include "SFG/Own2dEngine/Engine/audioManager.h"

#include "SFG/Own2dEngine/Engine/performance.h"

namespace SFG {
namespace Own2dEngine {
namespace Engine {

SFG::Own2dEngine::Logger::spdlogger AudioManager::logger_ = nullptr;
std::vector< AudioManager::AudioData > AudioManager::audios_;
PaStream* AudioManager::portAudioStream_ = nullptr;

int AudioManager::_numInputChannels_ = 0;
int AudioManager::_numOutputChannels_ = 2;
PaSampleFormat AudioManager::_sampleFormat_ = paFloat32;

PaStreamParameters AudioManager::_inputParameters_{ paNoDevice, AudioManager::_numInputChannels_, AudioManager::_sampleFormat_ };
PaStreamParameters AudioManager::_outputParameters_{ paNoDevice, AudioManager::_numOutputChannels_, AudioManager::_sampleFormat_ };
PaStreamFlags AudioManager::_streamFlags_ = paNoFlag;

double AudioManager::_sampleRate_ = 44100.0;
unsigned long AudioManager::_framesPerBuffer_ = 256;

void AudioManager::init() {
  AudioManager::logger_ = SFG::Own2dEngine::Logger::LoggerFactory::get_logger( "AudioManager" );
  AudioManager::logger_->trace( "init()" );

  int portAudioErrorCode;

  AudioManager::logger_->trace( "init - initializing PortAudio" );
  if( ( portAudioErrorCode = Pa_Initialize() ) != paNoError ) {
    spdlog::error( "init - error initializing PortAudio: {:d} ({:s})", portAudioErrorCode, Pa_GetErrorText( portAudioErrorCode ) );
  }

  AudioManager::logger_->trace( "init()~" );
}

void AudioManager::GetAudioInfos() {
  AudioManager::logger_->trace( "GetAudioInfos()" );

  int portAudioErrorCode;

  int numDevices = Pa_GetDeviceCount();
  if( numDevices < 0 ) {
    spdlog::error( "GetAudioInfos - error getting device count: {:d} ({:s})", numDevices, Pa_GetErrorText( numDevices ) );
  } else {
    PaDeviceInfo const* deviceInfo;
    PaHostApiInfo const* hostApiInfo;
    for( int i = 0; i < numDevices; i++ ) {
      deviceInfo = Pa_GetDeviceInfo( i );
      hostApiInfo = Pa_GetHostApiInfo( deviceInfo->hostApi );
      AudioManager::logger_->info( "GetAudioInfos - index {:d}:", i );
      AudioManager::logger_->info( "GetAudioInfos -   - name: {:?}", deviceInfo->name );
      AudioManager::logger_->info( "GetAudioInfos -   - hostApi: {:d} ({:?})", deviceInfo->hostApi, hostApiInfo->name );
      AudioManager::logger_->info( "GetAudioInfos -   - maxInputChannels: {:d}", deviceInfo->maxInputChannels );
      AudioManager::logger_->info( "GetAudioInfos -   - maxOutputChannels: {:d}", deviceInfo->maxOutputChannels );
      AudioManager::logger_->info( "GetAudioInfos -   - defaultLowInputLatency: {:f}", deviceInfo->defaultLowInputLatency );
      AudioManager::logger_->info( "GetAudioInfos -   - defaultLowOutputLatency: {:f}", deviceInfo->defaultLowOutputLatency );
      AudioManager::logger_->info( "GetAudioInfos -   - defaultHighInputLatency: {:f}", deviceInfo->defaultHighInputLatency );
      AudioManager::logger_->info( "GetAudioInfos -   - defaultHighOutputLatency: {:f}", deviceInfo->defaultHighOutputLatency );
      AudioManager::logger_->info( "GetAudioInfos -   - defaultSampleRate: {:f}", deviceInfo->defaultSampleRate );
    }
  }

  AudioManager::logger_->trace( "GetAudioInfos()~" );
}

void AudioManager::Start( PaDeviceIndex inputDeviceIndex, PaDeviceIndex outputDeviceIndex ) {
  AudioManager::logger_->trace( "Start( inputDeviceIndex: {:d}, outputDeviceIndex: {:d} )", inputDeviceIndex, outputDeviceIndex );

  int portAudioErrorCode;

  AudioManager::Stop();

  double sampleRate = AudioManager::_sampleRate_;

  if( inputDeviceIndex == paNoDevice || outputDeviceIndex == paNoDevice ) {
    if( ( portAudioErrorCode = Pa_OpenDefaultStream( &AudioManager::portAudioStream_,
                                                     AudioManager::_numInputChannels_,
                                                     AudioManager::_numOutputChannels_,
                                                     AudioManager::_sampleFormat_,
                                                     sampleRate,
                                                     AudioManager::_framesPerBuffer_,
                                                     AudioManager::ThreadRun,
                                                     nullptr ) )
        != paNoError ) {
      spdlog::error( "Start - error opening PortAudio stream: {:d} ({:s})", portAudioErrorCode, Pa_GetErrorText( portAudioErrorCode ) );
    }
  } else {
    PaDeviceInfo const* deviceInfo;

    deviceInfo = Pa_GetDeviceInfo( inputDeviceIndex );
    std::memset( &AudioManager::_inputParameters_, 0, sizeof( AudioManager::_inputParameters_ ) );
    if( deviceInfo ) {
      AudioManager::_inputParameters_.device = inputDeviceIndex;
      AudioManager::_inputParameters_.channelCount = std::max( 0, deviceInfo->maxInputChannels );
      AudioManager::_inputParameters_.sampleFormat = AudioManager::_sampleFormat_;
      sampleRate = std::min( sampleRate, deviceInfo->defaultSampleRate );
    }

    deviceInfo = Pa_GetDeviceInfo( outputDeviceIndex );
    std::memset( &AudioManager::_inputParameters_, 0, sizeof( AudioManager::_inputParameters_ ) );
    if( deviceInfo ) {
      AudioManager::_outputParameters_.device = outputDeviceIndex;
      AudioManager::_outputParameters_.channelCount = std::max( 2, deviceInfo->maxInputChannels );
      AudioManager::_outputParameters_.sampleFormat = AudioManager::_sampleFormat_;
      sampleRate = std::min( sampleRate, deviceInfo->defaultSampleRate );
    }

    if( ( portAudioErrorCode = Pa_OpenStream( &AudioManager::portAudioStream_,
                                              &AudioManager::_inputParameters_,
                                              &AudioManager::_outputParameters_,
                                              AudioManager::_sampleFormat_,
                                              AudioManager::_sampleRate_,
                                              AudioManager::_framesPerBuffer_,
                                              AudioManager::ThreadRun,
                                              nullptr ) )
        != paNoError ) {
      spdlog::error( "Start - error opening PortAudio stream: {:d} ({:s})", portAudioErrorCode, Pa_GetErrorText( portAudioErrorCode ) );
    }
  }

  if( AudioManager::portAudioStream_ ) {
    if( ( portAudioErrorCode = Pa_StartStream( AudioManager::portAudioStream_ ) ) != paNoError ) {
      spdlog::error( "Start - error starting PortAudio stream: {:d} ({:s})", portAudioErrorCode, Pa_GetErrorText( portAudioErrorCode ) );
    }
  }

  AudioManager::logger_->trace( "Start()~" );
}

void AudioManager::CreateAudio( std::string const& tag,
                                AudioManager::AudioType type,
                                uint16_t numChannels,
                                std::vector< float > audioSamples,
                                double origSampleRate ) {
  AudioManager::logger_->trace( "CreateAudio( tag: {:?}, type: {:d}, numChannels: {:d}, audioSamples: [{:n}], origSampleRate: {:f} )",
                                tag,
                                static_cast< uint16_t >( type ),
                                numChannels,
                                audioSamples,
                                origSampleRate );

  std::vector< float > newSamples;
  {
    PaStreamInfo const* streamInfo = Pa_GetStreamInfo( AudioManager::portAudioStream_ );
    Resample( audioSamples, origSampleRate, numChannels, streamInfo->sampleRate, newSamples );
  }
  // todo: fixme: fix this
  std::function< std::vector< float >( AudioManager::AudioData& ) > callback = []( AudioManager::AudioData& data ) {
    std::vector< float > ret;
    ret.reserve( data.numChannels );

    if( data.sampleIndex < data.audioSamples.size() ) {
      for( int i = 0; i < data.numChannels; i++ ) {
        ret.push_back( data.audioSamples[i + data.sampleIndex] );
      }
      data.sampleIndex = data.sampleIndex + data.numChannels;

      if( data.type == AudioManager::AudioType::BGM ) {
        data.sampleIndex = data.sampleIndex % data.audioSamples.size();
      }
    }
    return ret;
  };
  AudioManager::audios_.emplace_back( tag, type, numChannels, newSamples, 0, callback );

  AudioManager::logger_->trace( "CreateAudio()~" );
}

void AudioManager::StopAudio( std::string const& tag ) {
  AudioManager::logger_->trace( "StopAudio()" );

  for( int i = 0; i < AudioManager::audios_.size(); i++ ) {
    auto& item = AudioManager::audios_[i];

    // check if audio is done playing
    if( item.tag == tag ) {
      AudioManager::logger_->trace( "StopAudio - erasing AudioData index {:d}", i );
      AudioManager::audios_.erase( AudioManager::audios_.begin() + i );
      i--;
      continue;
    }
  }

  AudioManager::logger_->trace( "StopAudio()~" );
}

void AudioManager::Stop() {
  AudioManager::logger_->trace( "Stop()" );

  int portAudioErrorCode;

  if( AudioManager::portAudioStream_ ) {
    if( ( portAudioErrorCode = Pa_AbortStream( AudioManager::portAudioStream_ ) ) != paNoError ) {
      spdlog::error( "Stop - error aborting PortAudio stream: {:d} ({:s})", portAudioErrorCode, Pa_GetErrorText( portAudioErrorCode ) );
    } else {
      AudioManager::portAudioStream_ = nullptr;
    }
  }

  AudioManager::logger_->trace( "Stop()~" );
}

void AudioManager::Shutdown() {
  AudioManager::logger_->trace( "Shutdown()" );

  int portAudioErrorCode;

  AudioManager::Stop();

  AudioManager::logger_->trace( "Shutdown - clearing {:d} AudioDatas", AudioManager::audios_.size() );
  AudioManager::audios_.clear();

  AudioManager::logger_->trace( "Shutdown - terminating PortAudio" );
  if( ( portAudioErrorCode = Pa_Terminate() ) != paNoError ) {
    spdlog::error( "Shutdown - error terminating PortAudio: {:d} ({:s})", portAudioErrorCode, Pa_GetErrorText( portAudioErrorCode ) );
  }

  AudioManager::logger_->trace( "Shutdown()~" );
}

int AudioManager::ThreadRun( void const* input,
                             void* output,
                             unsigned long frameCount,
                             PaStreamCallbackTimeInfo const* timeInfo,
                             PaStreamCallbackFlags statusFlags,
                             void* userData ) {
  /*
  AudioManager::logger_->trace( "ThreadRun( input: {:p}, output: {:p}, frameCount: {:d}, timeInfo: {:p}, statusFlags: {:#x}, userData: {:p} )",
                                static_cast< void const* >( input ),
                                static_cast< void* >( output ),
                                frameCount,
                                static_cast< void const* >( timeInfo ),
                                statusFlags,
                                static_cast< void* >( userData ) );
  */
  Performance::startTiming( "Audio" );

  auto customClamp = []( float a, float b ) {
    float num = a + b;
    float upperBound = 1.0f;
    float lowerBound = -1.0f;
    return static_cast< float >( std::min( upperBound, std::max( lowerBound, a + b ) ) );
  };
  float* out = static_cast< float* >( output );
  for( uint64_t i = 0; i < frameCount; i++ ) {
    // stereo
    float outL = 0;
    float outR = 0;
    for( int i = 0; i < AudioManager::audios_.size(); i++ ) {
      auto& item = AudioManager::audios_[i];

      // check if audio is done playing
      if( item.sampleIndex >= item.audioSamples.size() ) {
        AudioManager::logger_->trace( "ThreadRun - erasing AudioData index {:d}", i );
        AudioManager::audios_.erase( AudioManager::audios_.begin() + i );
        i--;
        continue;
      }

      std::vector< float > samples = item.callback( item );
      if( samples.size() == 2 ) {
        outL = customClamp( outL, samples[0] );
        outR = customClamp( outR, samples[1] );
      } else if( samples.size() == 1 ) {
        outL = customClamp( outL, samples[0] );
        outR = customClamp( outR, samples[0] );
      }
    }
    out[i * 2] = outL;
    out[i * 2 + 1] = outR;
  }

  Performance::endTiming( "Audio" );
  /*
  AudioManager::logger_->trace( "ThreadRun()~" );
  */
  return paContinue;
}

}  // namespace Engine
}  // namespace Own2dEngine
}  // namespace SFG
