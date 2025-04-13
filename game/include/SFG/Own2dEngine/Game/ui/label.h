#ifndef ENGINE_UI_LABEL_H_
#define ENGINE_UI_LABEL_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SFG/Own2dEngine/Game/ui/widget.h>
#include <SFG/Own2dEngine/Logger/_include.h>
#include <vector>

namespace SFG {
namespace Own2dEngine {
namespace Game {
namespace UI {

class Label : public Widget {
  using _base_ = Widget;
  using _base_::_base_;

  public:
  enum class HorizontalAlignment { Left, Centered, Right };
  enum class VerticalAlignment { Top, Centered, Bottom };

  public:
  Label( std::string const& text, Widget* parent = nullptr, SDL_FRect position = { 0, 0, 0, 0 } );
  virtual ~Label();

  public:
  void OnRender( SDL_Renderer* renderer ) override;

  std::string GetText();
  void SetText( std::string const& value );
  Label::HorizontalAlignment GetHorizontalAlignment();
  void SetHorizontalAlignment( Label::HorizontalAlignment value );
  Label::VerticalAlignment GetVerticalAlignment();
  void SetVerticalAlignment( Label::VerticalAlignment value );
  std::string GetFontFile();
  void SetFontFile( std::string const& value );
  int32_t GetFontSize();
  void SetFontSize( int32_t value );
  SDL_Color GetFontColourActive();
  void SetFontColourActive( SDL_Color const& value );
  SDL_Color GetFontColourInactive();
  void SetFontColourInactive( SDL_Color const& value );

  protected:
  void ReopenFont();

  protected:
  std::string text_ = "";
  Label::HorizontalAlignment horizontalAlignment_ = Label::HorizontalAlignment::Left;
  Label::VerticalAlignment verticalAlignment_ = Label::VerticalAlignment::Top;
  std::string fontFile_ = "Resources/Fonts/NotoSansMono-Regular.ttf";
  int32_t fontSize_ = 18;
  SDL_Color fontColourActive_ = { 0xff, 0xff, 0xff, 0xff };
  SDL_Color fontColourInactive_ = { 0xff, 0xff, 0xff, 0x80 };

  private:
  SFG::Own2dEngine::Logger::spdlogger logger_;
  TTF_Font* font_ = nullptr;
  SDL_Texture* textTexture_ = nullptr;
  SDL_FRect textTextureSize_;
};

}  // namespace UI
}  // namespace Game
}  // namespace Own2dEngine
}  // namespace SFG

#endif /* ENGINE_UI_LABEL_H_ */
