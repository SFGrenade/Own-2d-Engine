#ifndef _ENGINE_SCRIPT_H_
#define _ENGINE_SCRIPT_H_

#include "_globals/moreChrono.h"


namespace SFG {
namespace Engine {

class Script {
  public:
  Script();
  virtual ~Script();

  virtual void start();
  virtual void frame_update();
  virtual void logic_update( std::chrono::secondsLongDouble const& deltaTime );
  virtual void end();
};

}  // namespace Engine
}  // namespace SFG

#endif /* _ENGINE_SCRIPT_H_ */
