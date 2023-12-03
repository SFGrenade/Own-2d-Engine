#include "engine/scriptcollider.h"

#include "_globals/list_contains.h"
#include "engine/scriptmanager.h"
#include "engine/sdlwindow.h"


SFG::Engine::ScriptCollider::ScriptCollider()
    : _base_(), isTrigger_( false ), enteredCollidersFromAbove_(), enteredCollidersFromBelow_(), enteredCollidersFromLeft_(), enteredCollidersFromRight_() {}

SFG::Engine::ScriptCollider::~ScriptCollider() {
  enteredCollidersFromAbove_.clear();
  enteredCollidersFromBelow_.clear();
  enteredCollidersFromLeft_.clear();
  enteredCollidersFromRight_.clear();
}

void SFG::Engine::ScriptCollider::logic_update( std::chrono::secondsLongDouble const& deltaTime ) {
  // what is this degeneracy
  for( int i = 0; i < this->scriptList_->size(); ++i ) {
    SFG::Engine::Script* script = this->scriptList_->at( i );
    if( script == this ) {
      continue;
    }
    SFG::Engine::ScriptCollider* colliderScript = dynamic_cast< SFG::Engine::ScriptCollider* >( script );
    if( colliderScript == nullptr ) {
      continue;
    }
    long double rect1X = this->position_.x;
    long double rect1Y = this->position_.y;
    long double rect1W = this->size_.x;
    long double rect1H = this->size_.y;
    long double rect1DeltaX = this->velocity_.x * deltaTime.count();
    long double rect1DeltaY = this->velocity_.y * deltaTime.count();
    long double rect2X = colliderScript->position_.x;
    long double rect2Y = colliderScript->position_.y;
    long double rect2W = colliderScript->size_.x;
    long double rect2H = colliderScript->size_.y;

    long double distanceBottom1ToTop2 = ( rect1Y + rect1H ) - rect2Y;
    long double distanceTop1ToBottom2 = rect1Y - ( rect2Y + rect2H );
    long double distanceRight1ToLeft2 = ( rect1X + rect1W ) - rect2X;
    long double distanceLeft1ToRight2 = rect1X - ( rect2X + rect2W );

    long double offsetDistanceBottom1ToTop2 = ( ( rect1Y + rect1H ) + rect1DeltaY ) - rect2Y;
    long double offsetDistanceTop1ToBottom2 = ( rect1Y + rect1DeltaY ) - ( rect2Y + rect2H );
    long double offsetDistanceRight1ToLeft2 = ( ( rect1X + rect1W ) + rect1DeltaX ) - rect2X;
    long double offsetDistanceLeft1ToRight2 = ( rect1X + rect1DeltaX ) - ( rect2X + rect2W );

    if( ( distanceBottom1ToTop2 >= 0.0L )     // rect1 enters rect2 from above
        && ( distanceTop1ToBottom2 <= 0.0L )  // rect1 enters rect2 from below
        && ( distanceRight1ToLeft2 >= 0.0L )  // rect1 enters rect2 from left
        && ( distanceLeft1ToRight2 <= 0.0L )  // rect1 enters rect2 from right
    ) {
      // spdlog::debug( fmt::runtime( "Collision between {:p} and {:p}" ), static_cast< void* >( this ), static_cast< void* >( colliderScript ) );
      if( list_contains< SFG::Engine::ScriptCollider* >( this->enteredCollidersFromAbove_, colliderScript ) ) {
        this->position_.y = colliderScript->position_.y - this->size_.y;
        this->velocity_.y = std::min( this->velocity_.y, 0.0L );
      } else if( list_contains< SFG::Engine::ScriptCollider* >( this->enteredCollidersFromBelow_, colliderScript ) ) {
        this->position_.y = colliderScript->position_.y + colliderScript->size_.y;
        this->velocity_.y = std::max( this->velocity_.y, 0.0L );
      } else if( list_contains< SFG::Engine::ScriptCollider* >( this->enteredCollidersFromLeft_, colliderScript ) ) {
        this->position_.x = colliderScript->position_.x - this->size_.x;
        this->velocity_.x = std::min( this->velocity_.x, 0.0L );
      } else if( list_contains< SFG::Engine::ScriptCollider* >( this->enteredCollidersFromRight_, colliderScript ) ) {
        this->position_.x = colliderScript->position_.x + colliderScript->size_.x;
        this->velocity_.x = std::max( this->velocity_.x, 0.0L );
      }
    } else if( ( offsetDistanceBottom1ToTop2 >= 0.0L )  // offset
               && ( distanceTop1ToBottom2 < 0.0L )      // normal
               && ( distanceRight1ToLeft2 > 0.0L )      // normal
               && ( distanceLeft1ToRight2 < 0.0L )      // normal
    ) {
      // spdlog::debug( fmt::runtime( "{:p} is about to run into {:p} from above" ), static_cast< void* >( this ), static_cast< void* >( colliderScript ) );
      this->enteredCollidersFromAbove_.push_back( colliderScript );
    } else if( ( distanceBottom1ToTop2 > 0.0L )            // normal
               && ( offsetDistanceTop1ToBottom2 <= 0.0L )  // offset
               && ( distanceRight1ToLeft2 > 0.0L )         // normal
               && ( distanceLeft1ToRight2 < 0.0L )         // normal
    ) {
      // spdlog::debug( fmt::runtime( "{:p} is about to run into {:p} from below" ), static_cast< void* >( this ), static_cast< void* >( colliderScript ) );
      this->enteredCollidersFromBelow_.push_back( colliderScript );
    } else if( ( distanceBottom1ToTop2 > 0.0L )            // normal
               && ( distanceTop1ToBottom2 < 0.0L )         // normal
               && ( offsetDistanceRight1ToLeft2 >= 0.0L )  // offset
               && ( distanceLeft1ToRight2 < 0.0L )         // normal
    ) {
      // spdlog::debug( fmt::runtime( "{:p} is about to run into {:p} from left" ), static_cast< void* >( this ), static_cast< void* >( colliderScript ) );
      this->enteredCollidersFromLeft_.push_back( colliderScript );
    } else if( ( distanceBottom1ToTop2 > 0.0L )            // normal
               && ( distanceTop1ToBottom2 < 0.0L )         // normal
               && ( distanceRight1ToLeft2 > 0.0L )         // normal
               && ( offsetDistanceLeft1ToRight2 <= 0.0L )  // offset
    ) {
      // spdlog::debug( fmt::runtime( "{:p} is about to run into {:p} from right" ), static_cast< void* >( this ), static_cast< void* >( colliderScript ) );
      this->enteredCollidersFromRight_.push_back( colliderScript );
    } else {
      if( std::find( this->enteredCollidersFromAbove_.begin(), this->enteredCollidersFromAbove_.end(), colliderScript )
          != this->enteredCollidersFromAbove_.end() ) {
        this->enteredCollidersFromAbove_.erase( std::find( this->enteredCollidersFromAbove_.begin(), this->enteredCollidersFromAbove_.end(), colliderScript ) );
      }
      if( std::find( this->enteredCollidersFromBelow_.begin(), this->enteredCollidersFromBelow_.end(), colliderScript )
          != this->enteredCollidersFromBelow_.end() ) {
        this->enteredCollidersFromBelow_.erase( std::find( this->enteredCollidersFromBelow_.begin(), this->enteredCollidersFromBelow_.end(), colliderScript ) );
      }
      if( std::find( this->enteredCollidersFromLeft_.begin(), this->enteredCollidersFromLeft_.end(), colliderScript )
          != this->enteredCollidersFromLeft_.end() ) {
        this->enteredCollidersFromLeft_.erase( std::find( this->enteredCollidersFromLeft_.begin(), this->enteredCollidersFromLeft_.end(), colliderScript ) );
      }
      if( std::find( this->enteredCollidersFromRight_.begin(), this->enteredCollidersFromRight_.end(), colliderScript )
          != this->enteredCollidersFromRight_.end() ) {
        this->enteredCollidersFromRight_.erase( std::find( this->enteredCollidersFromRight_.begin(), this->enteredCollidersFromRight_.end(), colliderScript ) );
      }
    }
  }

  _base_::logic_update( deltaTime );
}
