#ifndef _CONTENT_DEBUGINFO_H_
#define _CONTENT_DEBUGINFO_H_

#include "_globals/spdlogInclude.h"
#include "engine/script.h"


namespace SFG {
namespace Content {

struct DebugInfoStruct {
  std::string message_;
  bool drawNew_;
  SDL_Texture* texture_;
  SDL_Rect textureRect_;
};

class DebugInfo : public SFG::Engine::Script {
  public:
  DebugInfo( SFG::Engine::SdlWindow* sdlWindow );
  virtual ~DebugInfo();

  virtual void start();

  virtual void frame_update( SDL_Renderer const* renderer );
  virtual void input_update( SDL_Event const& input );
  virtual void logic_update( std::chrono::secondsLongDouble const& deltaTime );
  virtual void fixed_update();
  virtual void network_update();  // todo

  virtual void end();

  private:
  void set_debugInfo_topLeft( std::string const& debugInfo );
  void set_debugInfo_topRight( std::string const& debugInfo );
  void set_debugInfo_bottomLeft( std::string const& debugInfo );
  void set_debugInfo_bottomRight( std::string const& debugInfo );
  void renderDebugInfo( DebugInfoStruct& debugInfo, SDL_Renderer const* renderer );

  private:
  spdlogger logger_;

  bool rendering_;
  uint16_t countDown1s_;
  DebugInfoStruct debugInfoTopLeft_;
  DebugInfoStruct debugInfoTopRight_;
  DebugInfoStruct debugInfoBottomLeft_;
  DebugInfoStruct debugInfoBottomRight_;

  TTF_Font* sdlFont_;
};

}  // namespace Content
}  // namespace SFG

#endif /* _CONTENT_DEBUGINFO_H_ */
