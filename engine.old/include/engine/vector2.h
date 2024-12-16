#ifndef _ENGINE_VECTOR2_H_
#define _ENGINE_VECTOR2_H_

namespace SFG {
namespace Engine {

class Vector2 {
  public:
  Vector2( long double x, long double y );
  Vector2( SFG::Engine::Vector2 const& other );

  long double x();
  void set_x( long double new_x );
  long double y();
  void set_y( long double new_y );

  SFG::Engine::Vector2& operator=( SFG::Engine::Vector2 const& other );

  SFG::Engine::Vector2& operator+=( SFG::Engine::Vector2 const& other );
  SFG::Engine::Vector2& operator-=( SFG::Engine::Vector2 const& other );
  SFG::Engine::Vector2& operator*=( long double mult );
  SFG::Engine::Vector2& operator/=( long double div );

  SFG::Engine::Vector2 operator+( SFG::Engine::Vector2 const& other ) const;
  SFG::Engine::Vector2 operator-( SFG::Engine::Vector2 const& other ) const;
  SFG::Engine::Vector2 operator*( long double mult ) const;
  SFG::Engine::Vector2 operator/( long double div ) const;

  bool operator<( SFG::Engine::Vector2 const& other ) const;
  bool operator>( SFG::Engine::Vector2 const& other ) const;
  bool operator<=( SFG::Engine::Vector2 const& other ) const;
  bool operator>=( SFG::Engine::Vector2 const& other ) const;

  bool operator==( SFG::Engine::Vector2 const& other ) const;
  bool operator!=( SFG::Engine::Vector2 const& other ) const;

  protected:
  long double x_;
  long double y_;
};

}  // namespace Engine
}  // namespace SFG

#endif /* _ENGINE_VECTOR2_H_ */
