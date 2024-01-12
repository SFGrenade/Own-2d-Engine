#include "engine/scriptcollider.h"

#include "_globals/list_contains.h"
#include "_globals/num_ops.h"
#include "engine/scriptmanager.h"
#include "engine/sdlwindow.h"


SFG::Engine::ScriptCollider::ScriptCollider() : _base_(), isTrigger_( false ), isTouching_( false ), isColliding_( false ) {}

SFG::Engine::ScriptCollider::~ScriptCollider() {}

void SFG::Engine::ScriptCollider::logic_update( std::chrono::secondsLongDouble const& deltaTime ) {
  bool touchingAtLeastOne = false;
  bool collidingAtLeastOne = false;

  for( int i = 0; i < this->scriptList_->size(); i++ ) {  // maybe this is a race condition with begin and end or something, we'll see
    SFG::Engine::Script* script = ( *( this->scriptList_ ) )[i];
    if( script == this ) {
      // don't check against ourselves
      continue;
    }
    SFG::Engine::ScriptCollider* other = dynamic_cast< SFG::Engine::ScriptCollider* >( script );
    if( other == nullptr ) {
      // don't check against non-colliders
      continue;
    }

    // y position
    if( ( other->isTrigger_ ? true : ( this->velocity_.y() > 0.0L ) ) && this->get_bottom() >= other->get_top()
        && this->get_middle().y() <= other->get_middle().y() ) {
      if( this->get_right() > other->get_left() && this->get_left() < other->get_right() ) {
        touchingAtLeastOne = true;
        if( !other->isTrigger_ ) {
          this->velocity_.set_y( 0.0L );
        }
        other->interact( this, SFG::Engine::InteractSide::Top );
      }
    } else if( ( other->isTrigger_ ? true : ( this->velocity_.y() < 0.0L ) ) && this->get_top() <= other->get_bottom()
               && this->get_middle().y() > other->get_middle().y() ) {
      if( this->get_right() > other->get_left() && this->get_left() < other->get_right() ) {
        touchingAtLeastOne = true;
        if( !other->isTrigger_ ) {
          this->velocity_.set_y( 0.0L );
        }
        other->interact( this, SFG::Engine::InteractSide::Bottom );
      }
    }
    // x position
    if( ( other->isTrigger_ ? true : ( this->velocity_.x() > 0.0L ) ) && this->get_right() >= other->get_left()
        && this->get_middle().x() <= other->get_middle().x() ) {
      if( this->get_bottom() > other->get_top() && this->get_top() < other->get_bottom() ) {
        touchingAtLeastOne = true;
        if( !other->isTrigger_ ) {
          this->velocity_.set_x( 0.0L );
        }
        other->interact( this, SFG::Engine::InteractSide::Left );
      }
    } else if( ( other->isTrigger_ ? true : ( this->velocity_.x() < 0.0L ) ) && this->get_left() <= other->get_right()
               && this->get_middle().x() > other->get_middle().x() ) {
      if( this->get_bottom() > other->get_top() && this->get_top() < other->get_bottom() ) {
        touchingAtLeastOne = true;
        if( !other->isTrigger_ ) {
          this->velocity_.set_x( 0.0L );
        }
        other->interact( this, SFG::Engine::InteractSide::Right );
      }
    }
  }
  this->isTouching_ = collidingAtLeastOne ? false : touchingAtLeastOne;
  this->isColliding_ = collidingAtLeastOne;
  _base_::logic_update( deltaTime );
}

void SFG::Engine::ScriptCollider::interact( SFG::Engine::ScriptCollider const* contact, SFG::Engine::InteractSide interactionSide ) {}
