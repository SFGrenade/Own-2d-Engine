#ifndef ENGINE_AUDIOMANAGER_H_
#define ENGINE_AUDIOMANAGER_H_

#include <SFG/Own2dEngine/Logger/_include.h>
#include <functional>
#include <list>
#include <portaudio.h>
#include <string>

#include "SFG/Own2dEngine/Engine/resample/resample.h"

namespace SFG {
namespace Own2dEngine {
namespace Engine {

typedef int16_t AudioStreamType;
typedef int64_t HigherAudioStreamType;

class AudioManager {
  public:
  enum class AudioType : uint16_t { BGM, SFX };

  private:
  struct AudioData {
    std::string tag;
    AudioType type;
    uint16_t numChannels;
    std::vector< AudioStreamType > audioSamples;
    uint64_t sampleIndex;
    std::function< std::vector< AudioStreamType >( AudioManager::AudioData& ) > callback;
  };

  public:
  static void init();

  static void GetAudioInfos();
  static void Start( PaDeviceIndex inputDeviceIndex = paNoDevice, PaDeviceIndex outputDeviceIndex = paNoDevice );
  static void CreateAudio( std::string const& tag,
                           AudioManager::AudioType type,
                           uint16_t numChannels,
                           std::vector< AudioStreamType > audioSamples,
                           double origSampleRate );
  static void StopAudio( std::string const& tag );
  static void Stop();
  static void Shutdown();

  public:
  template < typename T >
  static void Resample( std::vector< T >& from, uint32_t fromSr, uint32_t toSr, std::vector< T >& to );

  private:
  static int ThreadRun( void const* input,
                        void* output,
                        unsigned long frameCount,
                        PaStreamCallbackTimeInfo const* timeInfo,
                        PaStreamCallbackFlags statusFlags,
                        void* userData );

  private:
  static SFG::Own2dEngine::Logger::spdlogger logger_;
  static std::vector< AudioManager::AudioData > audios_;
  static PaStream* portAudioStream_;

  static int _numInputChannels_;
  static int _numOutputChannels_;
  static PaSampleFormat _sampleFormat_;

  static PaStreamParameters _inputParameters_;
  static PaStreamParameters _outputParameters_;
  static PaStreamFlags _streamFlags_;

  static double _sampleRate_;
  static unsigned long _framesPerBuffer_;
};

template < typename T >
void AudioManager::Resample( std::vector< T >& from, uint32_t fromSr, uint32_t toSr, std::vector< T >& to ) {
  AudioManager::logger_->trace( "Resample( from: [{:d} samples], fromSr: {:d}, toSr: {:d} )", from.size(), fromSr, toSr );

  AudioManager::logger_->trace( "Resample - from: {}", fmt::join( from, ", " ) );

  std::vector< double > fromDouble;
  std::vector< double > toDouble;

  fromDouble.reserve( from.size() );
  for( uint64_t i = 0; i < from.size(); i++ ) {
    double val = static_cast< double >( from[i] );
    if( val > 0 ) {
      fromDouble.push_back( val / static_cast< double >( std::numeric_limits< T >::max() ) );
    } else if( val < 0 ) {
      fromDouble.push_back( ( -val ) / static_cast< double >( std::numeric_limits< T >::min() ) );
    } else {
      fromDouble.push_back( 0.0 );
    }
  }

  AudioManager::logger_->trace( "Resample - fromDouble: {}", fmt::join( fromDouble, ", " ) );

  AudioManager::logger_->trace( "Resample - calling resample" );
  Resample::resample< double >( toSr, fromSr, fromDouble, toDouble );

  AudioManager::logger_->trace( "Resample - toDouble: {}", fmt::join( toDouble, ", " ) );

  to.clear();
  to.reserve( toDouble.size() );
  for( uint64_t i = 0; i < toDouble.size(); i++ ) {
    if( toDouble[i] > 0 ) {
      to.push_back( static_cast< T >( toDouble[i] * static_cast< double >( std::numeric_limits< T >::max() ) ) );
    } else if( toDouble[i] < 0 ) {
      to.push_back( static_cast< T >( ( -toDouble[i] ) * static_cast< double >( std::numeric_limits< T >::min() ) ) );
    } else {
      to.push_back( 0 );
    }
  }

  AudioManager::logger_->trace( "Resample - to: {}", fmt::join( to, ", " ) );

  AudioManager::logger_->trace( "Resample()~" );
}

}  // namespace Engine
}  // namespace Own2dEngine
}  // namespace SFG

#endif /* ENGINE_AUDIOMANAGER_H_ */
