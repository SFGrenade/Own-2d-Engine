#pragma once

// Including our headers
#include "../globals.h"

namespace SFG {
namespace Utils {
static std::mutex _pointerCounterMtx;
static std::map<void*, uint64_t> _pointerCounter;

template <typename T>
class SmartPointer {
   private:
    T* _pointer;

    void AddToMap() {
        if (_pointer == nullptr) return;
        _pointerCounterMtx.lock();
        if (_pointerCounter.find(_pointer) == _pointerCounter.end()) _pointerCounter[_pointer] = 0;
        _pointerCounter[_pointer] += 1;
        _pointerCounterMtx.unlock();
    }
    void RemoveFromMap() {
        if (_pointer == nullptr) return;
        _pointerCounterMtx.lock();
        _pointerCounter[_pointer] -= 1;
        if (_pointerCounter[_pointer] > 0) {
            _pointerCounterMtx.unlock();
            return;
        }
        _pointerCounter.erase(_pointer);
        _pointerCounterMtx.unlock();
        delete _pointer;
        _pointer = nullptr;
    }

   public:
    SmartPointer(T* pointer = nullptr) {
        _pointer = pointer;
        AddToMap();
    }
    SmartPointer(const SmartPointer<T>& pointer) {
        _pointer = pointer._pointer;
        AddToMap();
    }
    ~SmartPointer() { RemoveFromMap(); }

    T* get() { return _pointer; }

    uint64_t count() { return _pointerCounter[_pointer]; }

    SmartPointer<T>& operator=(T* other) {
        RemoveFromMap();
        _pointer = other;
        AddToMap();
        return *this;
    }

    SmartPointer<T>& operator=(const SmartPointer<T>& other) {
        RemoveFromMap();
        _pointer = other._pointer;
        AddToMap();
        return *this;
    }

    T* operator->() const { return (T*)_pointer; }

    explicit operator bool() const { return _pointer; }

    bool operator==(std::nullptr_t) const { return _pointer; }

    bool operator!=(std::nullptr_t) const { return !(_pointer); }
};
}  // namespace Utils
}  // namespace SFG
