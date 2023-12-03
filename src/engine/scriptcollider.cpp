#include "engine/scriptcollider.h"

#include "_globals/list_contains.h"
#include "engine/scriptmanager.h"
#include "engine/sdlwindow.h"


SFG::Engine::ScriptCollider::ScriptCollider() : _base_(), isTrigger_( false ), isTouching_( false ), isColliding_( false ) {}

SFG::Engine::ScriptCollider::~ScriptCollider() {}

void SFG::Engine::ScriptCollider::logic_update( std::chrono::secondsLongDouble const& deltaTime ) {
  bool touchingAtLeastOne = false;
  bool collidingAtLeastOne = false;

  bool horizontalOverlap;
  bool verticalOverlap;
  // ==0 means bottom border touching other top border; >0 means bottom border below other top border
  long double distanceThisBottomToOtherTop;
  // ==0 means top border touching other bottom border; <0 means top border below other bottom border
  long double distanceThisTopToOtherBottom;
  // ==0 means right border touching other left border; >0 means right border below other left border
  long double distanceThisRightToOtherLeft;
  // ==0 means left border touching other right border; <0 means left border below other right border
  long double distanceThisLeftToOtherRight;
  bool thisTouchingOrCollidingOtherTop;
  bool thisTouchingOrCollidingOtherBottom;
  bool thisTouchingOrCollidingOtherLeft;
  bool thisTouchingOrCollidingOtherRight;
  bool thisCollidingOtherTop;
  bool thisCollidingOtherBottom;
  bool thisCollidingOtherLeft;
  bool thisCollidingOtherRight;
  bool doThisYAdjustment;
  bool doThisXAdjustment;

  for( SFG::Engine::Script* script : *( this->scriptList_ ) ) {  // maybe this is a race condition with begin and end or something, we'll see
    if( script == this ) {
      continue;
    }
    SFG::Engine::ScriptCollider* other = dynamic_cast< SFG::Engine::ScriptCollider* >( script );
    if( other == nullptr ) {
      continue;
    }
    horizontalOverlap = ( this->position_.x < ( other->position_.x + other->size_.x ) ) && ( ( this->position_.x + this->size_.x ) > other->position_.x );
    verticalOverlap = ( this->position_.y < ( other->position_.y + other->size_.y ) ) && ( ( this->position_.y + this->size_.y ) > other->position_.y );
    if( !horizontalOverlap && !verticalOverlap ) {
      continue;
    }

    distanceThisBottomToOtherTop = ( this->position_.y + this->size_.y ) - other->position_.y;
    distanceThisTopToOtherBottom = this->position_.y - ( other->position_.y + other->size_.y );
    distanceThisRightToOtherLeft = ( this->position_.x + this->size_.x ) - other->position_.x;
    distanceThisLeftToOtherRight = this->position_.x - ( other->position_.x + other->size_.x );

    thisTouchingOrCollidingOtherTop = ( this->velocity_.y >= 0.0L ) && ( distanceThisBottomToOtherTop >= 0.0L )
                                      && ( distanceThisBottomToOtherTop <= ( ( this->size_.y + other->size_.y ) / 2.0L ) );
    thisTouchingOrCollidingOtherBottom = ( this->velocity_.y <= 0.0L ) && ( distanceThisTopToOtherBottom <= 0.0L )
                                         && ( distanceThisTopToOtherBottom >= ( ( this->size_.y + other->size_.y ) / -2.0L ) );
    thisTouchingOrCollidingOtherLeft = ( this->velocity_.x >= 0.0L ) && ( distanceThisRightToOtherLeft >= 0.0L )
                                       && ( distanceThisRightToOtherLeft <= ( ( this->size_.x + other->size_.x ) / 2.0L ) );
    thisTouchingOrCollidingOtherRight = ( this->velocity_.x <= 0.0L ) && ( distanceThisLeftToOtherRight <= 0.0L )
                                        && ( distanceThisLeftToOtherRight >= ( ( this->size_.x + other->size_.x ) / -2.0L ) );
    if( !thisTouchingOrCollidingOtherTop && !thisTouchingOrCollidingOtherBottom && !thisTouchingOrCollidingOtherLeft && !thisTouchingOrCollidingOtherRight ) {
      continue;
    }

    thisCollidingOtherTop = thisTouchingOrCollidingOtherTop && ( this->velocity_.y > 0.0L ) && ( distanceThisBottomToOtherTop > 0.0L );
    thisCollidingOtherBottom = thisTouchingOrCollidingOtherBottom && ( this->velocity_.y < 0.0L ) && ( distanceThisTopToOtherBottom < 0.0L );
    thisCollidingOtherLeft = thisTouchingOrCollidingOtherLeft && ( this->velocity_.x > 0.0L ) && ( distanceThisRightToOtherLeft > 0.0L );
    thisCollidingOtherRight = thisTouchingOrCollidingOtherRight && ( this->velocity_.x < 0.0L ) && ( distanceThisLeftToOtherRight < 0.0L );

    doThisYAdjustment = false;
    doThisXAdjustment = false;

    if( thisCollidingOtherTop ) {
      doThisYAdjustment = ( abs( distanceThisBottomToOtherTop ) < abs( distanceThisRightToOtherLeft ) )
                          && ( abs( distanceThisBottomToOtherTop ) < abs( distanceThisLeftToOtherRight ) );
      doThisXAdjustment = !doThisYAdjustment;
    }
    if( thisCollidingOtherBottom ) {
      doThisYAdjustment = ( abs( distanceThisTopToOtherBottom ) < abs( distanceThisRightToOtherLeft ) )
                          && ( abs( distanceThisTopToOtherBottom ) < abs( distanceThisLeftToOtherRight ) );
      doThisXAdjustment = !doThisYAdjustment;
    }
    if( thisCollidingOtherLeft ) {
      doThisXAdjustment = ( abs( distanceThisRightToOtherLeft ) < abs( distanceThisBottomToOtherTop ) )
                          && ( abs( distanceThisRightToOtherLeft ) < abs( distanceThisTopToOtherBottom ) );
      doThisYAdjustment = !doThisXAdjustment;
    }
    if( thisCollidingOtherRight ) {
      doThisXAdjustment = ( abs( distanceThisLeftToOtherRight ) < abs( distanceThisBottomToOtherTop ) )
                          && ( abs( distanceThisLeftToOtherRight ) < abs( distanceThisTopToOtherBottom ) );
      doThisYAdjustment = !doThisXAdjustment;
    }

    if( horizontalOverlap ) {
      // check for up and down touching
      if( thisTouchingOrCollidingOtherTop ) {
        // this is touching or colliding with other
        touchingAtLeastOne = true;
        this->velocity_.y = std::min( this->velocity_.y, 0.0L );
        if( thisCollidingOtherTop ) {
          // actual collision, not only touching
          collidingAtLeastOne = true;
          if( doThisYAdjustment ) {
            this->position_.y = other->position_.y - this->size_.y;
          }
        }
      }
      if( thisTouchingOrCollidingOtherBottom ) {
        // this is touching or colliding with other
        touchingAtLeastOne = true;
        this->velocity_.y = std::max( this->velocity_.y, 0.0L );
        if( thisCollidingOtherBottom ) {
          // actual collision, not only touching
          collidingAtLeastOne = true;
          if( doThisYAdjustment ) {
            this->position_.y = other->position_.y + other->size_.y;
          }
        }
      }
    }
    if( verticalOverlap ) {
      // check for left and right touching
      if( thisTouchingOrCollidingOtherLeft ) {
        // this is touching or colliding with other
        touchingAtLeastOne = true;
        this->velocity_.x = std::min( this->velocity_.x, 0.0L );
        if( thisCollidingOtherLeft ) {
          // actual collision, not only touching
          collidingAtLeastOne = true;
          if( doThisXAdjustment ) {
            this->position_.x = other->position_.x - this->size_.x;
          }
        }
      }
      if( thisTouchingOrCollidingOtherRight ) {
        // this is touching or colliding with other
        touchingAtLeastOne = true;
        this->velocity_.x = std::max( this->velocity_.x, 0.0L );
        if( thisCollidingOtherRight ) {
          // actual collision, not only touching
          collidingAtLeastOne = true;
          if( doThisXAdjustment ) {
            this->position_.x = other->position_.x + other->size_.x;
          }
        }
      }
    }
  }
  this->isTouching_ = collidingAtLeastOne ? false : touchingAtLeastOne;
  this->isColliding_ = collidingAtLeastOne;

  _base_::logic_update( deltaTime );
}
