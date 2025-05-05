#ifndef ENGINE_AUDIOMANAGER_H_
#define ENGINE_AUDIOMANAGER_H_

#include <SFG/Own2dEngine/Logger/_include.h>
#include <functional>
#include <portaudio.h>
#include <samplerate.h>
#include <string>
#include <vector>

namespace SFG {
namespace Own2dEngine {
namespace Engine {

class AudioManager {
  public:
  enum class AudioType : uint16_t { BGM, SFX };

  private:
  struct AudioData {
    std::string tag;
    AudioType type;
    uint16_t numChannels;
    std::vector< float > audioSamples;
    uint64_t sampleIndex;
    std::function< std::vector< float >( AudioManager::AudioData& ) > callback;
  };

  public:
  static void init();

  static void GetAudioInfos();
  static void Start( PaDeviceIndex inputDeviceIndex = paNoDevice, PaDeviceIndex outputDeviceIndex = paNoDevice );
  static void CreateAudio( std::string const& tag,
                           AudioManager::AudioType type,
                           uint16_t numChannels,
                           std::vector< float > audioSamples,
                           double origSampleRate );
  static void StopAudio( std::string const& tag );
  static void Stop();
  static void Shutdown();

  public:
  template < typename T >
  static void Resample( std::vector< T >& from, uint32_t fromSr, uint16_t fromChannelCount, uint32_t toSr, std::vector< T >& to );

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
void AudioManager::Resample( std::vector< T >& from, uint32_t fromSr, uint16_t fromChannelCount, uint32_t toSr, std::vector< T >& to ) {
  AudioManager::logger_->trace( "Resample( from: [{:d} samples], fromSr: {:d}, toSr: {:d} )", from.size(), fromSr, toSr );

  AudioManager::logger_->trace( "Resample - from: {}", fmt::join( from, ", " ) );

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

  AudioManager::logger_->trace( "Resample - to: {}", fmt::join( to, ", " ) );

  AudioManager::logger_->trace( "Resample()~" );
}

}  // namespace Engine
}  // namespace Own2dEngine
}  // namespace SFG

#endif /* ENGINE_AUDIOMANAGER_H_ */
