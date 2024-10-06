#ifndef _CONTENT_AUDIOREGION_H_
#define _CONTENT_AUDIOREGION_H_

#include "_globals/align.h"
#include "_globals/sdlInclude.h"
#include "_globals/spdlogInclude.h"
#include "engine/scriptcollider.h"


namespace SFG {
namespace Content {

class AudioRegion : public SFG::Engine::ScriptCollider {
  using _base_ = SFG::Engine::ScriptCollider;
  using _base_::_base_;

  public:
  AudioRegion();
  virtual ~AudioRegion();

  virtual void frame_update( SDL_Renderer* renderer ) override;
  virtual void logic_update( std::chrono::secondsLongDouble const& deltaTime ) override;

  virtual void interact( SFG::Engine::ScriptCollider const* contact, SFG::Engine::InteractSide interactionSide ) override;

  void set_audio_file_path( std::string const& audioFilePath );

  private:
  spdlogger logger_;

  bool rendering_;
  SFG_ENGINE_ALIGN( 64 ) SDL_Rect audioRegionRect_;  // used for graphics and logic
  SDL_Texture* audioRegionTexture_;

  std::string audioFilePath_;
};

}  // namespace Content
}  // namespace SFG

#endif /* _CONTENT_AUDIOREGION_H_ */
