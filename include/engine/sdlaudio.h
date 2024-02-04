#ifndef _ENGINE_SDLAUDIO_H_
#define _ENGINE_SDLAUDIO_H_

#include <list>
#include <string>

#include "_globals/sdlInclude.h"
#include "_globals/spdlogInclude.h"


namespace SFG {
namespace Engine {

class SdlEngine;

class SdlAudio {
  public:
  SdlAudio( SFG::Engine::SdlEngine* sdlEngine );
  ~SdlAudio();

  void initialize_sdl_audio();

  void play_background_music( std::string const& path );
  void play_sound_effect( std::string const& path );

  private:
  spdlogger logger_;

  SFG::Engine::SdlEngine* sdlEngine_;

  std::string lastPlayedAudio_;
  Mix_Chunk* sdlMixChunkBgm1_;
  Mix_Chunk* sdlMixChunkBgm2_;
  std::list< Mix_Chunk* > sdlMixChunkSoundEffects_;
};

}  // namespace Engine
}  // namespace SFG

#endif /* _ENGINE_SDLAUDIO_H_ */
