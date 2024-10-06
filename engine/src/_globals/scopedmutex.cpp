#include "_globals/scopedmutex.h"

ScopedMutex::ScopedMutex( std::mutex* mutex ) : mutex_( mutex ) {
  this->mutex_->lock();
}

ScopedMutex::~ScopedMutex() {
  this->mutex_->unlock();
}
