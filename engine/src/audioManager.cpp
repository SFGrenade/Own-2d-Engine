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
PaSampleFormat AudioManager::_sampleFormat_ = paInt16;

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

void AudioManager::CreateAudio( std::vector< int16_t > audioSamples, std::function< void( AudioManager::AudioData& ) > callback ) {
  AudioManager::logger_->trace( "CreateAudio( audioSamples: [{:d} items], callback )", audioSamples.size() );

  // todo: fixme: fix this
  AudioManager::audios_.emplace_back( audioSamples, 0, callback );

  AudioManager::logger_->trace( "CreateAudio()~" );
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

  AudioStreamType* out = static_cast< AudioStreamType* >( output );
  for( uint64_t i = 0; i < frameCount; i++ ) {
    // stereo
    out[i * 2] = 0;
    out[i * 2 + 1] = 0;
  }

  /*
  AudioManager::logger_->trace( "ThreadRun()~" );
  */
  return paContinue;
}

}  // namespace Engine
}  // namespace Own2dEngine
}  // namespace SFG
