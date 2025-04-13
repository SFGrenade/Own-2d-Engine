#include "SFG/Own2dEngine/Game/ui/label.h"

// our shit
#include "SFG/Own2dEngine/Game/inputManager.h"
#include "SFG/Own2dEngine/Game/performance.h"

namespace SFG {
namespace Own2dEngine {
namespace Game {
namespace UI {

Label::Label( std::string const& text, Widget* parent, SDL_FRect position )
    : _base_( parent, position ), logger_( SFG::Own2dEngine::Logger::LoggerFactory::get_logger( "Label" ) ), text_( text ) {
  logger_->trace( "Label( text: '{}' )", text );

  ReopenFont();

  logger_->trace( "Label()~" );
}

Label::~Label() {
  logger_->trace( "~Label()" );

  if( font_ ) {
    TTF_CloseFont( font_ );
    font_ = nullptr;
  }
  if( textTexture_ ) {
    SDL_DestroyTexture( textTexture_ );
    textTexture_ = nullptr;
  }

  logger_->trace( "~Label()~" );
}

void Label::OnRender( SDL_Renderer* renderer ) {
  if( !IsVisibleHierarchy() ) {
    return;
  }

  // destroy old texture
  if( textTexture_ ) {
    SDL_DestroyTexture( textTexture_ );
    textTexture_ = nullptr;
  }
  // render new texture
  if( font_ && renderer ) {
    SDL_Surface* textSurface;
    if( IsActiveHierarchy() ) {
      textSurface = TTF_RenderUTF8_Blended_Wrapped( font_, text_.c_str(), fontColourActive_, global_position_.w );
    } else {
      textSurface = TTF_RenderUTF8_Blended_Wrapped( font_, text_.c_str(), fontColourInactive_, global_position_.w );
    }

    textTextureSize_.w = textSurface->w;
    textTextureSize_.h = textSurface->h;

    textTexture_ = SDL_CreateTextureFromSurface( renderer, textSurface );
    SDL_FreeSurface( textSurface );
  }
  // align texture inside bounding box
  if( horizontalAlignment_ == Label::HorizontalAlignment::Left ) {
    textTextureSize_.x = global_position_.x;
  } else if( horizontalAlignment_ == Label::HorizontalAlignment::Centered ) {
    textTextureSize_.x = global_position_.x + ( ( global_position_.w - textTextureSize_.w ) / 2.0f );
  } else if( horizontalAlignment_ == Label::HorizontalAlignment::Right ) {
    textTextureSize_.x = ( global_position_.x + global_position_.w ) - textTextureSize_.w;
  }
  if( verticalAlignment_ == Label::VerticalAlignment::Top ) {
    textTextureSize_.y = global_position_.y;
  } else if( verticalAlignment_ == Label::VerticalAlignment::Centered ) {
    textTextureSize_.y = global_position_.y + ( ( global_position_.h - textTextureSize_.h ) / 2.0f );
  } else if( verticalAlignment_ == Label::VerticalAlignment::Bottom ) {
    textTextureSize_.y = ( global_position_.y + global_position_.h ) - textTextureSize_.h;
  }
  // copy texture
  if( renderer && textTexture_ ) {
    SDL_RenderCopyF( renderer, textTexture_, nullptr, &textTextureSize_ );
  }

  _base_::OnRender( renderer );
}

std::string Label::GetText() {
  return text_;
}

void Label::SetText( std::string const& value ) {
  text_ = value;
}

Label::HorizontalAlignment Label::GetHorizontalAlignment() {
  return horizontalAlignment_;
}

void Label::SetHorizontalAlignment( Label::HorizontalAlignment value ) {
  horizontalAlignment_ = value;
}

Label::VerticalAlignment Label::GetVerticalAlignment() {
  return verticalAlignment_;
}

void Label::SetVerticalAlignment( Label::VerticalAlignment value ) {
  verticalAlignment_ = value;
}

std::string Label::GetFontFile() {
  return fontFile_;
}

void Label::SetFontFile( std::string const& value ) {
  fontFile_ = value;

  ReopenFont();
}

int32_t Label::GetFontSize() {
  return fontSize_;
}

void Label::SetFontSize( int32_t value ) {
  fontSize_ = value;

  ReopenFont();
}

SDL_Color Label::GetFontColourActive() {
  return fontColourActive_;
}

void Label::SetFontColourActive( SDL_Color const& value ) {
  fontColourActive_.r = value.r;
  fontColourActive_.g = value.g;
  fontColourActive_.b = value.b;
  fontColourActive_.a = value.a;
}

SDL_Color Label::GetFontColourInactive() {
  return fontColourInactive_;
}

void Label::SetFontColourInactive( SDL_Color const& value ) {
  fontColourInactive_.r = value.r;
  fontColourInactive_.g = value.g;
  fontColourInactive_.b = value.b;
  fontColourInactive_.a = value.a;
}

void Label::ReopenFont() {
  if( font_ ) {
    TTF_CloseFont( font_ );
    font_ = nullptr;
  }
  font_ = TTF_OpenFont( fontFile_.c_str(), fontSize_ );
}

}  // namespace UI
}  // namespace Game
}  // namespace Own2dEngine
}  // namespace SFG
