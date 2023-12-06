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

  // true if aligning or intersecting, to set velocity
  bool thisTopAligningOtherBottom;
  bool thisLeftAligningOtherRight;
  bool thisRightAligningOtherLeft;
  bool thisBottomAligningOtherTop;

  // true if intersecting, to maybe set position
  bool thisTopIntersectingOtherBottom;
  bool thisLeftIntersectingOtherRight;
  bool thisRightIntersectingOtherLeft;
  bool thisBottomIntersectingOtherTop;

  SFG::Engine::Vector2 intersectionTopLeft( 0.0L, 0.0L );
  SFG::Engine::Vector2 intersectionBottomRight( 0.0L, 0.0L );
  SFG::Engine::Vector2 intersectionSize( 0.0L, 0.0L );
  bool verticalCollision;

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

    thisTopAligningOtherBottom = ( this->get_top() <= other->get_bottom() ) && ( this->get_middle().y() > other->get_middle().y() );
    thisLeftAligningOtherRight = ( this->get_left() <= other->get_right() ) && ( this->get_middle().x() > other->get_middle().x() );
    thisRightAligningOtherLeft = ( this->get_right() >= other->get_left() ) && ( this->get_middle().x() <= other->get_middle().x() );
    thisBottomAligningOtherTop = ( this->get_bottom() >= other->get_top() ) && ( this->get_middle().y() <= other->get_middle().y() );
    if( ( !thisTopAligningOtherBottom ) && ( !thisLeftAligningOtherRight ) && ( !thisRightAligningOtherLeft ) && ( !thisBottomAligningOtherTop ) ) {
      // no collision or touch at all
      continue;
    }

    intersectionTopLeft = SFG::Engine::Vector2( 0.0L, 0.0L );
    intersectionBottomRight = SFG::Engine::Vector2( 0.0L, 0.0L );
    if( thisTopAligningOtherBottom ) {
      intersectionTopLeft.set_y( this->get_top() );
      intersectionBottomRight.set_y( min( this->get_bottom(), other->get_bottom() ) );
    }
    if( thisLeftAligningOtherRight ) {
      intersectionTopLeft.set_x( this->get_left() );
      intersectionBottomRight.set_x( min( this->get_right(), other->get_right() ) );
    }
    if( thisRightAligningOtherLeft ) {
      intersectionTopLeft.set_x( max( this->get_left(), other->get_left() ) );
      intersectionBottomRight.set_x( this->get_right() );
    }
    if( thisBottomAligningOtherTop ) {
      intersectionTopLeft.set_y( max( this->get_top(), other->get_top() ) );
      intersectionBottomRight.set_y( this->get_bottom() );
    }
    intersectionSize = intersectionBottomRight - intersectionTopLeft;
    if( ( intersectionSize.x() == 0.0L ) && ( intersectionSize.y() == 0.0L ) ) {
      // single point collision doesn't count
      continue;
    }
    verticalCollision = intersectionSize.x() >= intersectionSize.y();

    thisTopIntersectingOtherBottom = ( this->get_top() < other->get_bottom() ) && ( this->get_middle().y() > other->get_middle().y() );
    thisLeftIntersectingOtherRight = ( this->get_left() < other->get_right() ) && ( this->get_middle().x() > other->get_middle().x() );
    thisRightIntersectingOtherLeft = ( this->get_right() > other->get_left() ) && ( this->get_middle().x() <= other->get_middle().x() );
    thisBottomIntersectingOtherTop = ( this->get_bottom() > other->get_top() ) && ( this->get_middle().y() <= other->get_middle().y() );

    if( verticalCollision ) {
      if( thisTopAligningOtherBottom ) {
        touchingAtLeastOne = true;
        if( this->velocity_.y() < 0.0L ) {
          this->velocity_.set_y( 0.0L );
          if( thisTopIntersectingOtherBottom ) {
            collidingAtLeastOne = true;
            // adjust position
            this->position_.set_y( other->get_bottom() );
          }
        }
      } else if( thisBottomAligningOtherTop ) {
        touchingAtLeastOne = true;
        if( this->velocity_.y() > 0.0L ) {
          this->velocity_.set_y( 0.0L );
          if( thisBottomIntersectingOtherTop ) {
            collidingAtLeastOne = true;
            // adjust position
            this->position_.set_y( other->get_top() - this->size_.y() );
          }
        }
      }
    } else {
      if( thisLeftAligningOtherRight ) {
        touchingAtLeastOne = true;
        if( this->velocity_.x() < 0.0L ) {
          this->velocity_.set_x( 0.0L );
          if( thisLeftIntersectingOtherRight ) {
            collidingAtLeastOne = true;
            // adjust position
            this->position_.set_x( other->get_right() );
          }
        }
      } else if( thisRightAligningOtherLeft ) {
        touchingAtLeastOne = true;
        if( this->velocity_.x() > 0.0L ) {
          this->velocity_.set_x( 0.0L );
          if( thisRightIntersectingOtherLeft ) {
            collidingAtLeastOne = true;
            // adjust position
            this->position_.set_x( other->get_left() - this->size_.x() );
          }
        }
      }
    }
  }
  this->isTouching_ = collidingAtLeastOne ? false : touchingAtLeastOne;
  this->isColliding_ = collidingAtLeastOne;
  _base_::logic_update( deltaTime );
}
