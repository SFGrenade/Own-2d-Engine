#ifndef _GLOBALS_SCOPEDMUTEX_H_
#define _GLOBALS_SCOPEDMUTEX_H_

#include <mutex>

class ScopedMutex {
  public:
  ScopedMutex( std::mutex* mutex );
  ~ScopedMutex();

  private:
  __declspec( align( 64 ) ) std::mutex* mutex_;
};

#endif /* _GLOBALS_SCOPEDMUTEX_H_ */
