#include "engine/scriptcollider.h"

#include "_globals/list_contains.h"
#include "_globals/num_ops.h"
#include "engine/scriptmanager.h"
#include "engine/sdlwindow.h"


SFG::Engine::ScriptCollider::ScriptCollider() : _base_(), isStatic_( false ), isTrigger_( false ), isTouching_( false ), isColliding_( false ) {}

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

    if( this->isStatic_ && other->isStatic_ ) {
      // ignore two static colliders colliding
      continue;
    }

    // y position
    if( this->get_moved_bottom( deltaTime ) >= other->get_moved_top( deltaTime )
        && this->get_moved_middle( deltaTime ).y() <= other->get_moved_middle( deltaTime ).y() ) {
      if( this->get_right() > other->get_left() && this->get_left() < other->get_right() ) {
        touchingAtLeastOne = true;
        if( !other->isTrigger_ ) {
          if( this->get_moved_bottom( deltaTime ) > other->get_moved_top( deltaTime ) ) {
            // this will be inside other, reposition this
            collidingAtLeastOne = true;
            if( !this->isStatic_ ) {
              this->set_position( this->position_.x(), other->get_top() - this->size_.y() );
            }
          }
          this->velocity_.set_y( min( this->velocity_.y(), 0.0L ) );
        }
        other->interact( this, SFG::Engine::InteractSide::Top );
      }
    } else if( this->get_moved_top( deltaTime ) <= other->get_moved_bottom( deltaTime )
               && this->get_moved_middle( deltaTime ).y() > other->get_moved_middle( deltaTime ).y() ) {
      if( this->get_right() > other->get_left() && this->get_left() < other->get_right() ) {
        touchingAtLeastOne = true;
        if( !other->isTrigger_ ) {
          if( this->get_moved_top( deltaTime ) < other->get_moved_bottom( deltaTime ) ) {
            // this will be inside other, reposition this
            collidingAtLeastOne = true;
            if( !this->isStatic_ ) {
              this->set_position( this->position_.x(), other->get_bottom() );
            }
          }
          this->velocity_.set_y( max( this->velocity_.y(), 0.0L ) );
        }
        other->interact( this, SFG::Engine::InteractSide::Bottom );
      }
    }
    // x position
    if( this->get_moved_right( deltaTime ) >= other->get_moved_left( deltaTime )
        && this->get_moved_middle( deltaTime ).x() <= other->get_moved_middle( deltaTime ).x() ) {
      if( this->get_bottom() > other->get_top() && this->get_top() < other->get_bottom() ) {
        touchingAtLeastOne = true;
        if( !other->isTrigger_ ) {
          if( this->get_moved_right( deltaTime ) > other->get_moved_left( deltaTime ) ) {
            // this will be inside other, reposition this
            collidingAtLeastOne = true;
            if( !this->isStatic_ ) {
              this->set_position( other->get_left() - this->size_.x(), this->position_.y() );
            }
          }
          this->velocity_.set_x( min( this->velocity_.x(), 0.0L ) );
        }
        other->interact( this, SFG::Engine::InteractSide::Left );
      }
    } else if( this->get_moved_left( deltaTime ) <= other->get_moved_right( deltaTime )
               && this->get_moved_middle( deltaTime ).x() > other->get_moved_middle( deltaTime ).x() ) {
      if( this->get_bottom() > other->get_top() && this->get_top() < other->get_bottom() ) {
        touchingAtLeastOne = true;
        if( !other->isTrigger_ ) {
          if( this->get_moved_left( deltaTime ) < other->get_moved_right( deltaTime ) ) {
            // this will be inside other, reposition this
            collidingAtLeastOne = true;
            if( !this->isStatic_ ) {
              this->set_position( other->get_right(), this->position_.y() );
            }
          }
          this->velocity_.set_x( max( this->velocity_.x(), 0.0L ) );
        }
        other->interact( this, SFG::Engine::InteractSide::Right );
      }
    }
  }
  this->isTouching_ = collidingAtLeastOne ? false : touchingAtLeastOne;
  this->isColliding_ = collidingAtLeastOne;
  if( this->isStatic_ ) {
    this->velocity_.set_x( 0.0L );
    this->velocity_.set_y( 0.0L );
  }
  _base_::logic_update( deltaTime );
}

void SFG::Engine::ScriptCollider::interact( SFG::Engine::ScriptCollider const* contact, SFG::Engine::InteractSide interactionSide ) {}
