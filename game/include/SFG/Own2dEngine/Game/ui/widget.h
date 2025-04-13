#ifndef ENGINE_UI_WIDGET_H_
#define ENGINE_UI_WIDGET_H_

#include <SDL2/SDL.h>
#include <SFG/Own2dEngine/Logger/_include.h>
#include <vector>

namespace SFG {
namespace Own2dEngine {
namespace Game {
namespace UI {

class Widget {
  public:
  Widget( Widget* parent = nullptr, SDL_FRect position = { 0, 0, 0, 0 } );
  virtual ~Widget();

  public:
  virtual void OnLogic();
  virtual void OnRender( SDL_Renderer* renderer );

  void SetParent( Widget* parent );

  bool IsDebug();
  void SetDebug( bool value );

  bool IsVisibleHierarchy();
  bool IsVisible();
  void SetVisible( bool value );

  bool IsActiveHierarchy();
  bool IsActive();
  void SetActive( bool value );

  float GetXHierarchy();
  float GetYHierarchy();
  float GetWHierarchy();
  float GetHHierarchy();

  float GetX();
  void SetX( float value );
  float GetY();
  void SetY( float value );
  float GetW();
  void SetW( float value );
  float GetH();
  void SetH( float value );

  protected:
  void SetJustSwitchedVisible();
  void SetJustSwitchedActive();

  protected:
  bool cursorHovering_ = false;
  bool mbPressed_ = false;
  bool mbDown_ = false;

  protected:
  // required protected for rendering
  SDL_FRect global_position_;

  private:
  SFG::Own2dEngine::Logger::spdlogger logger_;

  Widget* parent_ = nullptr;
  std::vector< Widget* > children_;

  bool debug_ = false;
  bool visible_ = true;
  bool justSwitchedVisible_ = false;
  bool active_ = true;
  bool justSwitchedActive_ = false;
  SDL_FRect position_;  // this is relative to parent (all vals 0.0-1.0), unless you're topmost (parent == nullptr)
};

}  // namespace UI
}  // namespace Game
}  // namespace Own2dEngine
}  // namespace SFG

#endif /* ENGINE_UI_WIDGET_H_ */
