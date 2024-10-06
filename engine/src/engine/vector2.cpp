#include "engine/vector2.h"

SFG::Engine::Vector2::Vector2( long double x, long double y ) : x_( x ), y_( y ) {}

SFG::Engine::Vector2::Vector2( SFG::Engine::Vector2 const& other ) : x_( other.x_ ), y_( other.y_ ) {}

long double SFG::Engine::Vector2::x() {
  return this->x_;
}

void SFG::Engine::Vector2::set_x( long double new_x ) {
  this->x_ = new_x;
}

long double SFG::Engine::Vector2::y() {
  return this->y_;
}

void SFG::Engine::Vector2::set_y( long double new_y ) {
  this->y_ = new_y;
}

SFG::Engine::Vector2& SFG::Engine::Vector2::operator=( SFG::Engine::Vector2 const& other ) {
  if( this == &other )
    return ( *this );
  this->x_ = other.x_;
  this->y_ = other.y_;
  return ( *this );
}

SFG::Engine::Vector2& SFG::Engine::Vector2::operator+=( SFG::Engine::Vector2 const& other ) {
  this->x_ += other.x_;
  this->y_ += other.y_;
  return ( *this );
}

SFG::Engine::Vector2& SFG::Engine::Vector2::operator-=( SFG::Engine::Vector2 const& other ) {
  this->x_ -= other.x_;
  this->y_ -= other.y_;
  return ( *this );
}

SFG::Engine::Vector2& SFG::Engine::Vector2::operator*=( long double mult ) {
  this->x_ *= mult;
  this->y_ *= mult;
  return ( *this );
}

SFG::Engine::Vector2& SFG::Engine::Vector2::operator/=( long double div ) {
  this->x_ /= div;
  this->y_ /= div;
  return ( *this );
}

SFG::Engine::Vector2 SFG::Engine::Vector2::operator+( SFG::Engine::Vector2 const& other ) const {
  SFG::Engine::Vector2 ret = ( *this );
  ret += other;
  return ret;
}

SFG::Engine::Vector2 SFG::Engine::Vector2::operator-( SFG::Engine::Vector2 const& other ) const {
  SFG::Engine::Vector2 ret = ( *this );
  ret -= other;
  return ret;
}

SFG::Engine::Vector2 SFG::Engine::Vector2::operator*( long double mult ) const {
  SFG::Engine::Vector2 ret = ( *this );
  ret *= mult;
  return ret;
}

SFG::Engine::Vector2 SFG::Engine::Vector2::operator/( long double div ) const {
  SFG::Engine::Vector2 ret = ( *this );
  ret /= div;
  return ret;
}

bool SFG::Engine::Vector2::operator<( SFG::Engine::Vector2 const& other ) const {
  bool lt_b = ( this->x_ < other.x_ ) && ( this->y_ < other.y_ );
  bool lt_x = ( this->x_ < other.x_ ) && ( this->y_ <= other.y_ );
  bool lt_y = ( this->x_ <= other.x_ ) && ( this->y_ < other.y_ );
  return lt_b || lt_x || lt_y;
}

bool SFG::Engine::Vector2::operator>( SFG::Engine::Vector2 const& other ) const {
  return other < ( *this );
}

bool SFG::Engine::Vector2::operator<=( SFG::Engine::Vector2 const& other ) const {
  return !( ( *this ) > other );
}

bool SFG::Engine::Vector2::operator>=( SFG::Engine::Vector2 const& other ) const {
  return !( ( *this ) < other );
}

bool SFG::Engine::Vector2::operator==( SFG::Engine::Vector2 const& other ) const {
  return ( this->x_ == other.x_ ) && ( this->y_ == other.y_ );
}

bool SFG::Engine::Vector2::operator!=( SFG::Engine::Vector2 const& other ) const {
  return !( ( *this ) == other );
}
