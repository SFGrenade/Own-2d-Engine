#ifndef ENGINE_UI_BUTTON_H_
#define ENGINE_UI_BUTTON_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SFG/Own2dEngine/Game/ui/label.h>
#include <SFG/Own2dEngine/Logger/_include.h>
#include <vector>

namespace SFG {
namespace Own2dEngine {
namespace Game {
namespace UI {

class Button : public Label {
  using _base_ = Label;
  using _base_::_base_;

  public:
  Button( std::function< void() > const& callback, std::string const& text, Widget* parent = nullptr, SDL_FRect position = { 0, 0, 0, 0 } );
  virtual ~Button();

  public:
  void OnLogic() override;
  void OnRender( SDL_Renderer* renderer ) override;

  std::function< void() > GetCallback();
  void SetCallback( std::function< void() > const& value );
  SDL_Color GetBackgroundColourHovering();
  void SetBackgroundColourHovering( SDL_Color const& value );
  SDL_Color GetBackgroundColourPressed();
  void SetBackgroundColourPressed( SDL_Color const& value );
  SDL_Color GetBorderColourActive();
  void SetBorderColourActive( SDL_Color const& value );
  SDL_Color GetBorderColourInactive();
  void SetBorderColourInactive( SDL_Color const& value );

  protected:
  SDL_Color backgroundColourHovering_ = { 0xff, 0xff, 0xff, 0x40 };
  SDL_Color backgroundColourPressed_ = { 0x00, 0x00, 0xff, 0x80 };
  SDL_Color borderColourActive_ = { 0xff, 0xff, 0xff, 0xff };
  SDL_Color borderColourInactive_ = { 0xff, 0xff, 0xff, 0x80 };

  private:
  SFG::Own2dEngine::Logger::spdlogger logger_;
  std::function< void() > callback_;
};

}  // namespace UI
}  // namespace Game
}  // namespace Own2dEngine
}  // namespace SFG

#endif /* ENGINE_UI_BUTTON_H_ */
