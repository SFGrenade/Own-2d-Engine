#include "SFG/Own2dEngine/Game/audioManager.h"

// std lib
#include <fstream>

// dr_wav lib
#define DR_WAV_IMPLEMENTATION
#include <dr_wav.h>

// libsamplerate
#include <samplerate.h>

// our shit
#include "SFG/Own2dEngine/Game/performance.h"

namespace SFG {
namespace Own2dEngine {
namespace Game {

SFG::Own2dEngine::Logger::spdlogger AudioManager::logger_ = nullptr;
std::mutex AudioManager::audiosMutex_;
std::vector< AudioManager::AudioData > AudioManager::audios_;
PaStream* AudioManager::portAudioInOutputStream_ = nullptr;

int AudioManager::_numInputChannels_ = 1;
int AudioManager::_numOutputChannels_ = 2;
PaSampleFormat AudioManager::_sampleFormat_ = paFloat32;
PaStreamFlags AudioManager::_streamFlags_ = paNoFlag;

double AudioManager::_sampleRate_ = 48000.0;
unsigned long AudioManager::_framesPerBuffer_ = paFramesPerBufferUnspecified;

double AudioManager::volumeBgm_ = 0.5;
double AudioManager::volumeSfx_ = 0.5;
double AudioManager::volumeVoice_ = 0.5;

void AudioManager::init() {
  AudioManager::logger_ = SFG::Own2dEngine::Logger::LoggerFactory::get_logger( "AudioManager" );
  AudioManager::logger_->trace( "init()" );

  int portAudioErrorCode;

  AudioManager::logger_->trace( "init - initializing PortAudio" );
  if( ( portAudioErrorCode = Pa_Initialize() ) != paNoError ) {
    AudioManager::logger_->error( "init - error initializing PortAudio: {:d} ({:s})", portAudioErrorCode, Pa_GetErrorText( portAudioErrorCode ) );
  }

  AudioManager::logger_->trace( "init()~" );
}

void AudioManager::GetAudioInfos() {
  AudioManager::logger_->trace( "GetAudioInfos()" );

  int portAudioErrorCode;

  int numDevices = Pa_GetDeviceCount();
  if( numDevices < 0 ) {
    AudioManager::logger_->error( "GetAudioInfos - error getting device count: {:d} ({:s})", numDevices, Pa_GetErrorText( numDevices ) );
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

  if( inputDeviceIndex == paNoDevice ) {
    inputDeviceIndex = Pa_GetDefaultInputDevice();
  }
  if( outputDeviceIndex == paNoDevice ) {
    outputDeviceIndex = Pa_GetDefaultOutputDevice();
  }

  PaStreamParameters inputParameters;
  PaDeviceInfo const* inputDeviceInfo = Pa_GetDeviceInfo( inputDeviceIndex );
  if( inputDeviceInfo ) {
    inputParameters.device = inputDeviceIndex;
    inputParameters.channelCount = AudioManager::_numInputChannels_;
    inputParameters.suggestedLatency = inputDeviceInfo->defaultLowInputLatency;
    inputParameters.sampleFormat = AudioManager::_sampleFormat_;
    inputParameters.hostApiSpecificStreamInfo = nullptr;
  }
  PaStreamParameters outputParameters;
  PaDeviceInfo const* outputDeviceInfo = Pa_GetDeviceInfo( outputDeviceIndex );
  if( outputDeviceInfo ) {
    outputParameters.device = outputDeviceIndex;
    outputParameters.channelCount = AudioManager::_numOutputChannels_;
    outputParameters.suggestedLatency = outputDeviceInfo->defaultLowOutputLatency;
    outputParameters.sampleFormat = AudioManager::_sampleFormat_;
    outputParameters.hostApiSpecificStreamInfo = nullptr;
  }
  sampleRate = std::min( { sampleRate, inputDeviceInfo->defaultSampleRate, outputDeviceInfo->defaultSampleRate } );

  AudioManager::logger_
      ->trace( "Start - calling Pa_OpenStream({:p}, ({:d}, {:d}, {:d}, {:f}, {:p}), ({:d}, {:d}, {:d}, {:f}, {:p}), {:f}, {:d}, {:d}, func, {:p})",
               static_cast< void* >( &AudioManager::portAudioInOutputStream_ ),
               inputParameters.device,
               inputParameters.channelCount,
               inputParameters.sampleFormat,
               inputParameters.suggestedLatency,
               inputParameters.hostApiSpecificStreamInfo,
               outputParameters.device,
               outputParameters.channelCount,
               outputParameters.sampleFormat,
               outputParameters.suggestedLatency,
               outputParameters.hostApiSpecificStreamInfo,
               sampleRate,
               AudioManager::_framesPerBuffer_,
               AudioManager::_streamFlags_,
               static_cast< void* >( nullptr ) );
  if( ( portAudioErrorCode = Pa_OpenStream( &AudioManager::portAudioInOutputStream_,
                                            &inputParameters,
                                            &outputParameters,
                                            sampleRate,
                                            AudioManager::_framesPerBuffer_,
                                            AudioManager::_streamFlags_,
                                            AudioManager::PaInOutputCb,
                                            nullptr ) )
      != paNoError ) {
    AudioManager::logger_->error( "Start - error opening PortAudio input stream: {:d} ({:s})", portAudioErrorCode, Pa_GetErrorText( portAudioErrorCode ) );
  }

  if( AudioManager::portAudioInOutputStream_ ) {
    if( ( portAudioErrorCode = Pa_StartStream( AudioManager::portAudioInOutputStream_ ) ) != paNoError ) {
      AudioManager::logger_->error( "Start - error starting PortAudio input stream: {:d} ({:s})", portAudioErrorCode, Pa_GetErrorText( portAudioErrorCode ) );
    }
  }

  AudioManager::logger_->trace( "Start()~" );
}

void AudioManager::LoadAudioFile( std::string const& tag, AudioManager::AudioType type, std::string const& filepath ) {
  AudioManager::logger_->trace( "LoadAudioFile( tag: {:?}, type: {:d}, filepath: {:?} )", tag, static_cast< uint16_t >( type ), filepath );

  unsigned int channels;
  unsigned int sampleRate;
  drwav_uint64 totalPCMFrameCount;
  float* pSampleData = drwav_open_file_and_read_pcm_frames_f32( filepath.c_str(), &channels, &sampleRate, &totalPCMFrameCount, NULL );
  if( pSampleData != NULL ) {
    // No error opening and reading WAV file.
    std::vector< AudioStreamType > audioSamples( pSampleData, std::next( pSampleData, static_cast< std::ptrdiff_t >( totalPCMFrameCount * channels ) ) );

    drwav_free( pSampleData, NULL );

    CreateAudio( tag, type, channels, audioSamples, static_cast< double >( sampleRate ) );
  }

  AudioManager::logger_->trace( "LoadAudioFile()~" );
}

void AudioManager::CreateAudio( std::string const& tag,
                                AudioManager::AudioType type,
                                uint16_t numChannels,
                                std::vector< AudioStreamType > audioSamples,
                                double origSampleRate ) {
  AudioManager::logger_->trace( "CreateAudio( tag: {:?}, type: {:d}, numChannels: {:d}, audioSamples: [{:d} samples], origSampleRate: {:f} )",
                                tag,
                                static_cast< uint16_t >( type ),
                                numChannels,
                                audioSamples.size(),
                                origSampleRate );

  std::vector< AudioStreamType > newSamples;
  if( AudioManager::portAudioInOutputStream_ ) {
    Resample( audioSamples, origSampleRate, numChannels, Pa_GetStreamInfo( AudioManager::portAudioInOutputStream_ )->sampleRate, newSamples );
    std::scoped_lock _( AudioManager::audiosMutex_ );
    AudioManager::audios_.emplace_back( tag, type, numChannels, newSamples, 0 );
  }

  AudioManager::logger_->trace( "CreateAudio()~" );
}

void AudioManager::StopAudio( std::string const& tag ) {
  AudioManager::logger_->trace( "StopAudio()" );

  std::scoped_lock _( AudioManager::audiosMutex_ );
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

  if( AudioManager::portAudioInOutputStream_ ) {
    if( ( portAudioErrorCode = Pa_CloseStream( AudioManager::portAudioInOutputStream_ ) ) != paNoError ) {
      AudioManager::logger_->error( "Stop - error closing PortAudio stream: {:d} ({:s})", portAudioErrorCode, Pa_GetErrorText( portAudioErrorCode ) );
    } else {
      AudioManager::portAudioInOutputStream_ = nullptr;
    }
  }

  AudioManager::logger_->trace( "Stop()~" );
}

void AudioManager::Shutdown() {
  AudioManager::logger_->trace( "Shutdown()" );

  int portAudioErrorCode;

  AudioManager::Stop();

  std::scoped_lock _( AudioManager::audiosMutex_ );
  AudioManager::logger_->trace( "Shutdown - clearing {:d} AudioDatas", AudioManager::audios_.size() );
  AudioManager::audios_.clear();

  AudioManager::logger_->trace( "Shutdown - terminating PortAudio" );
  if( ( portAudioErrorCode = Pa_Terminate() ) != paNoError ) {
    AudioManager::logger_->error( "Shutdown - error terminating PortAudio: {:d} ({:s})", portAudioErrorCode, Pa_GetErrorText( portAudioErrorCode ) );
  }

  AudioManager::logger_->trace( "Shutdown()~" );
}

void AudioManager::Resample( std::vector< AudioStreamType >& from,
                             uint32_t fromSr,
                             uint16_t fromChannelCount,
                             uint32_t toSr,
                             std::vector< AudioStreamType >& to ) {
  AudioManager::logger_->trace( "Resample( from: [{:d} samples], fromSr: {:d}, fromChannelCount: {:d}, toSr: {:d} )",
                                from.size(),
                                fromSr,
                                fromChannelCount,
                                toSr );

  SRC_DATA conversionData;
  conversionData.src_ratio = static_cast< double >( toSr ) / static_cast< double >( fromSr );

  to.clear();
  to.resize( std::ceil( from.size() * conversionData.src_ratio ) );

  conversionData.input_frames = from.size() / fromChannelCount;
  conversionData.output_frames = to.size() / fromChannelCount;
  conversionData.data_in = from.data();
  conversionData.data_out = to.data();

  AudioManager::logger_->trace( "Resample - calling resample" );
  int resampleResult = src_simple( &conversionData, SRC_SINC_BEST_QUALITY, fromChannelCount );

  AudioManager::logger_->trace( "Resample - resampleResult: {:d}", resampleResult );
  AudioManager::logger_->trace( "Resample - input_frames_used: {:d}", conversionData.input_frames_used );
  AudioManager::logger_->trace( "Resample - output_frames_gen: {:d}", conversionData.output_frames_gen );

  AudioManager::logger_->trace( "Resample()~" );
}

int paDummyCb( void const*, void*, unsigned long, PaStreamCallbackTimeInfo const*, PaStreamCallbackFlags, void* ) {
  return paContinue;
}

std::pair< std::vector< PaDeviceIndex >, std::vector< PaDeviceIndex > > AudioManager::TestABunchOfShit() {
  AudioManager::logger_->trace( "TestABunchOfShit()" );

  std::vector< PaDeviceIndex > inputs;
  std::vector< PaDeviceIndex > outputs;

  int portAudioErrorCode;

  PaDeviceIndex deviceCount = Pa_GetDeviceCount();
  PaStreamParameters inputParameters;
  PaStreamParameters outputParameters;
  for( PaDeviceIndex inDev = 0; inDev < deviceCount; inDev++ ) {
    PaDeviceInfo const* inDeviceInfo = Pa_GetDeviceInfo( inDev );
    if( inDeviceInfo->maxInputChannels <= 0 ) {
      continue;
    }
    inputParameters.device = inDev;
    inputParameters.channelCount = AudioManager::_numInputChannels_;
    inputParameters.suggestedLatency = inDeviceInfo->defaultLowInputLatency;
    inputParameters.sampleFormat = AudioManager::_sampleFormat_;
    inputParameters.hostApiSpecificStreamInfo = nullptr;
    double inSampleRate = inDeviceInfo->defaultSampleRate;

    for( PaDeviceIndex outDev = 0; outDev < deviceCount; outDev++ ) {
      PaDeviceInfo const* outDeviceInfo = Pa_GetDeviceInfo( outDev );
      if( outDeviceInfo->maxOutputChannels <= 0 ) {
        continue;
      }
      outputParameters.device = outDev;
      outputParameters.channelCount = AudioManager::_numOutputChannels_;
      outputParameters.suggestedLatency = outDeviceInfo->defaultLowOutputLatency;
      outputParameters.sampleFormat = AudioManager::_sampleFormat_;
      outputParameters.hostApiSpecificStreamInfo = nullptr;
      double outSampleRate = outDeviceInfo->defaultSampleRate;

      double sampleRateToUse = std::min( inSampleRate, outSampleRate );

      if( ( portAudioErrorCode = Pa_IsFormatSupported( &inputParameters, &outputParameters, sampleRateToUse ) ) == paNoError ) {
        AudioManager::logger_->trace( "TestABunchOfShit - no error with: ( in: ({:d}, {:d}, {:d}, {:f}, {:p}), out: ({:d}, {:d}, {:d}, {:f}, {:p}), sr: {:f} )",
                                      inputParameters.device,
                                      inputParameters.channelCount,
                                      inputParameters.sampleFormat,
                                      inputParameters.suggestedLatency,
                                      inputParameters.hostApiSpecificStreamInfo,
                                      outputParameters.device,
                                      outputParameters.channelCount,
                                      outputParameters.sampleFormat,
                                      outputParameters.suggestedLatency,
                                      outputParameters.hostApiSpecificStreamInfo,
                                      sampleRateToUse );

        PaStream* tmpStream = nullptr;
        if( ( portAudioErrorCode
              = Pa_OpenStream( &tmpStream, &inputParameters, &outputParameters, sampleRateToUse, paFramesPerBufferUnspecified, paNoFlag, paDummyCb, nullptr ) )
            != paNoError ) {
          AudioManager::logger_->trace( "TestABunchOfShit - error actually opening stream: {:d}, {:s}",
                                        portAudioErrorCode,
                                        Pa_GetErrorText( portAudioErrorCode ) );
        } else {
          if( ( portAudioErrorCode = Pa_CloseStream( tmpStream ) ) != paNoError ) {
            AudioManager::logger_->trace( "TestABunchOfShit - error closing temporary stream: {:d}, {:s}",
                                          portAudioErrorCode,
                                          Pa_GetErrorText( portAudioErrorCode ) );
          }

          bool vectorHasInput = false;
          for( auto const& item : inputs ) {
            if( inDev == item ) {
              vectorHasInput = true;
            }
          }
          if( !vectorHasInput ) {
            inputs.push_back( inDev );
          }
          bool vectorHasOutput = false;
          for( auto const& item : outputs ) {
            if( outDev == item ) {
              vectorHasOutput = true;
            }
          }
          if( !vectorHasOutput ) {
            outputs.push_back( outDev );
          }
        }
      } else {
        AudioManager::logger_
            ->error( "TestABunchOfShit - {:d}, {:s} with: ( in: ({:d}, {:d}, {:d}, {:f}, {:p}), out: ({:d}, {:d}, {:d}, {:f}, {:p}), sr: {:f} )",
                     portAudioErrorCode,
                     Pa_GetErrorText( portAudioErrorCode ),
                     inputParameters.device,
                     inputParameters.channelCount,
                     inputParameters.sampleFormat,
                     inputParameters.suggestedLatency,
                     inputParameters.hostApiSpecificStreamInfo,
                     outputParameters.device,
                     outputParameters.channelCount,
                     outputParameters.sampleFormat,
                     outputParameters.suggestedLatency,
                     outputParameters.hostApiSpecificStreamInfo,
                     sampleRateToUse );
      }
    }
  }

  AudioManager::logger_->trace( "TestABunchOfShit()~" );
  return { inputs, outputs };
}

AudioStreamType customClamp( AudioStreamType a, AudioStreamType b ) {
  HigherAudioStreamType num = static_cast< HigherAudioStreamType >( a ) + static_cast< HigherAudioStreamType >( b );

  HigherAudioStreamType upperBound = 1.0;
  HigherAudioStreamType lowerBound = -1.0;

  return static_cast< AudioStreamType >( std::min< HigherAudioStreamType >( upperBound, std::max< HigherAudioStreamType >( lowerBound, a + b ) ) );
}

int AudioManager::PaInOutputCb( void const* input,
                                void* output,
                                unsigned long frameCount,
                                PaStreamCallbackTimeInfo const*,
                                PaStreamCallbackFlags statusFlags,
                                void* ) {
  // input is mono, so we can freely use frameCount
  AudioStreamType const* in = static_cast< AudioStreamType const* >( input );

  AudioStreamType* out = static_cast< AudioStreamType* >( output );
  for( uint64_t i = 0; i < frameCount; i++ ) {
    // output is stereo, so one left and one right
    AudioStreamType outSampleLeft, outSampleRight;
    outSampleLeft = outSampleRight = in[i] * AudioManager::volumeVoice_;
    {
      std::scoped_lock _( AudioManager::audiosMutex_ );
      for( int a = 0; a < AudioManager::audios_.size(); a++ ) {
        auto& item = AudioManager::audios_[a];

        // check if audio is done playing
        if( ( item.sampleIndex >= item.audioSamples.size() ) && ( item.type != AudioManager::AudioType::BGM ) ) {
          AudioManager::logger_->trace( "PaOutputCb - erasing AudioData index {:d}", a );
          AudioManager::audios_.erase( AudioManager::audios_.begin() + a );
          a--;
          continue;
        }

        item.sampleIndex = item.sampleIndex % item.audioSamples.size();

        double multiplier = 0.5;
        if( item.type == AudioManager::AudioType::BGM ) {
          multiplier = AudioManager::volumeBgm_;
        } else if( item.type == AudioManager::AudioType::SFX ) {
          multiplier = AudioManager::volumeSfx_;
        }

        if( item.numChannels == 1 ) {
          outSampleLeft = customClamp( outSampleLeft, item.audioSamples[item.sampleIndex] * multiplier );
          outSampleRight = customClamp( outSampleRight, item.audioSamples[item.sampleIndex] * multiplier );
          item.sampleIndex++;
        } else if( item.numChannels == 2 ) {
          outSampleLeft = customClamp( outSampleLeft, item.audioSamples[item.sampleIndex] * multiplier );
          outSampleRight = customClamp( outSampleRight, item.audioSamples[item.sampleIndex + 1] * multiplier );
          item.sampleIndex += 2;
        }
      }
    }
    out[( 2 * i ) + 0] = outSampleLeft;
    out[( 2 * i ) + 1] = outSampleRight;
  }

  return paContinue;
}

}  // namespace Game
}  // namespace Own2dEngine
}  // namespace SFG
