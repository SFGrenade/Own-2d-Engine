#include "SFG/Own2dEngine/Game/ui/widget.h"

// our shit
#include "SFG/Own2dEngine/Game/inputManager.h"
#include "SFG/Own2dEngine/Game/performance.h"

namespace SFG {
namespace Own2dEngine {
namespace Game {
namespace UI {

Widget::Widget( Widget* parent, SDL_FRect position ) : logger_( SFG::Own2dEngine::Logger::LoggerFactory::get_logger( "Widget" ) ), parent_( parent ) {
  logger_
      ->trace( "Widget( parent: {}, position: (x: {}, y: {}, w: {}, h: {}) )", static_cast< void* >( parent ), position.x, position.y, position.w, position.h );

  if( parent_ ) {
    parent_->children_.push_back( this );
  }

  position_.x = position.x;
  position_.y = position.y;
  position_.w = position.w;
  position_.h = position.h;

  logger_->trace( "Widget()~" );
}

Widget::~Widget() {
  logger_->trace( "~Widget()" );

  for( auto child : children_ ) {
    child->active_ = false;
    delete child;
  }
  children_.clear();

  logger_->trace( "~Widget()~" );
}

void Widget::OnLogic() {
  std::function< void() > resetValuesFunc = [this]() {
    // reset these values
    this->justSwitchedVisible_ = false;
    this->justSwitchedActive_ = false;
  };
  // do logic
  global_position_.x = GetXHierarchy();
  global_position_.y = GetYHierarchy();
  global_position_.w = GetWHierarchy();
  global_position_.h = GetHHierarchy();
  if( !IsVisibleHierarchy() ) {
    resetValuesFunc();
    return;
  }

  if( !( justSwitchedVisible_ || justSwitchedActive_ ) ) {
    cursorHovering_ = SDL_PointInFRect( SFG::Own2dEngine::Game::InputManager::GetMouse(), &global_position_ ) == SDL_TRUE;
    mbPressed_ = cursorHovering_ && SFG::Own2dEngine::Game::InputManager::IsMouseButtonPressed( 1 );
    mbDown_ = cursorHovering_ && SFG::Own2dEngine::Game::InputManager::IsMouseButtonDown( 1 );
  } else {
    cursorHovering_ = false;
    mbPressed_ = false;
    mbDown_ = false;
  }

  // then logic children
  for( auto& child : children_ ) {
    child->OnLogic();
  }
  resetValuesFunc();
}

void Widget::OnRender( SDL_Renderer* renderer ) {
  if( !IsVisibleHierarchy() ) {
    return;
  }
  // do rendering
  if( IsDebug() ) {
    if( IsActiveHierarchy() ) {
      SDL_SetRenderDrawColor( renderer, 0xff, 0x00, 0x00, 0xff );
    } else {
      SDL_SetRenderDrawColor( renderer, 0xff, 0x00, 0x00, 0x80 );
    }
    SDL_RenderDrawRectF( renderer, &global_position_ );
  }

  // then render children
  for( auto& child : children_ ) {
    child->OnRender( renderer );
  }
}

void Widget::SetParent( Widget* parent ) {
  // remove from old parent
  if( parent_ ) {
    for( int i = 0; i < parent_->children_.size(); i++ ) {
      if( parent_->children_[i] == this ) {
        parent_->children_.erase( parent_->children_.begin() + i );
        i--;
      }
    }
  }

  parent_ = parent;

  // add to new parent
  if( parent_ ) {
    parent_->children_.push_back( this );
  }
}

bool Widget::IsDebug() {
  return debug_;
}

void Widget::SetDebug( bool value ) {
  debug_ = value;
}

bool Widget::IsVisibleHierarchy() {
  return ( parent_ ? parent_->IsVisibleHierarchy() : true ) && IsVisible();
}

bool Widget::IsVisible() {
  return visible_;
}

void Widget::SetVisible( bool value ) {
  if( value && !visible_ ) {
    SetJustSwitchedVisible();
  }
  visible_ = value;
}

bool Widget::IsActiveHierarchy() {
  return ( parent_ ? parent_->IsActiveHierarchy() : true ) && IsActive();
}

bool Widget::IsActive() {
  return active_;
}

void Widget::SetActive( bool value ) {
  if( value && !active_ ) {
    SetJustSwitchedActive();
  }
  active_ = value;
}

float Widget::GetXHierarchy() {
  // position is relative to parent (all vals 0.0-1.0), unless you're topmost (parent == nullptr)
  if( parent_ == nullptr ) {
    return GetX();
  }
  float parentXOffset = parent_->GetXHierarchy();
  float thisXOffset = GetX() * parent_->GetWHierarchy();
  return parentXOffset + thisXOffset;
}

float Widget::GetYHierarchy() {
  // position is relative to parent (all vals 0.0-1.0), unless you're topmost (parent == nullptr)
  if( parent_ == nullptr ) {
    return GetY();
  }
  float parentYOffset = parent_->GetYHierarchy();
  float thisYOffset = GetY() * parent_->GetHHierarchy();
  return parentYOffset + thisYOffset;
}

float Widget::GetWHierarchy() {
  // position is relative to parent (all vals 0.0-1.0), unless you're topmost (parent == nullptr)
  if( parent_ == nullptr ) {
    return GetW();
  }
  return parent_->GetWHierarchy() * GetW();
}

float Widget::GetHHierarchy() {
  // position is relative to parent (all vals 0.0-1.0), unless you're topmost (parent == nullptr)
  if( parent_ == nullptr ) {
    return GetH();
  }
  return parent_->GetHHierarchy() * GetH();
}

float Widget::GetX() {
  return position_.x;
}

void Widget::SetX( float value ) {
  position_.x = value;
}

float Widget::GetY() {
  return position_.y;
}

void Widget::SetY( float value ) {
  position_.y = value;
}

float Widget::GetW() {
  return position_.w;
}

void Widget::SetW( float value ) {
  position_.w = value;
}

float Widget::GetH() {
  return position_.h;
}

void Widget::SetH( float value ) {
  position_.h = value;
}

void Widget::SetJustSwitchedVisible() {
  justSwitchedVisible_ = true;
  for( auto& child : children_ ) {
    child->SetJustSwitchedVisible();
  }
}

void Widget::SetJustSwitchedActive() {
  justSwitchedActive_ = true;
  for( auto& child : children_ ) {
    child->SetJustSwitchedActive();
  }
}

}  // namespace UI
}  // namespace Game
}  // namespace Own2dEngine
}  // namespace SFG
