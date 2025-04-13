#ifndef ENGINE_AUDIOMANAGER_H_
#define ENGINE_AUDIOMANAGER_H_

#include <SFG/Own2dEngine/Logger/_include.h>
#include <functional>
#include <portaudio.h>
#include <queue>
#include <string>
#include <vector>

namespace SFG {
namespace Own2dEngine {
namespace Game {

typedef float AudioStreamType;
typedef double HigherAudioStreamType;

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
  };

  public:
  static void init();

  static void GetAudioInfos();
  static void Start( PaDeviceIndex inputDeviceIndex = paNoDevice, PaDeviceIndex outputDeviceIndex = paNoDevice );
  static void LoadAudioFile( std::string const& tag, AudioManager::AudioType type, std::string const& filepath );
  static void CreateAudio( std::string const& tag,
                           AudioManager::AudioType type,
                           uint16_t numChannels,
                           std::vector< AudioStreamType > audioSamples,
                           double origSampleRate );
  static void StopAudio( std::string const& tag );
  static void Stop();
  static void Shutdown();

  static void Resample( std::vector< AudioStreamType >& from, uint32_t fromSr, uint16_t fromChannelCount, uint32_t toSr, std::vector< AudioStreamType >& to );

  static std::pair< std::vector< PaDeviceIndex >, std::vector< PaDeviceIndex > > TestABunchOfShit();

  private:
  static int PaInOutputCb( void const* input,
                           void* output,
                           unsigned long frameCount,
                           PaStreamCallbackTimeInfo const* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void* userData );

  private:
  static SFG::Own2dEngine::Logger::spdlogger logger_;
  static std::mutex audiosMutex_;
  static std::vector< AudioManager::AudioData > audios_;
  static PaStream* portAudioInOutputStream_;

  static int _numInputChannels_;
  static int _numOutputChannels_;
  static PaSampleFormat _sampleFormat_;
  static PaStreamFlags _streamFlags_;

  static double _sampleRate_;
  static unsigned long _framesPerBuffer_;

  static double volumeBgm_;
  static double volumeSfx_;
  static double volumeVoice_;
};

}  // namespace Game
}  // namespace Own2dEngine
}  // namespace SFG

#endif /* ENGINE_AUDIOMANAGER_H_ */
