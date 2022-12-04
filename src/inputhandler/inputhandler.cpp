#include "inputhandler.h"

// Include our overall Objects
#include "../globals.h"

namespace SFG {
spdlogger InputHandler::logger = nullptr;
SDL_Event InputHandler::sdlEvent;
std::vector<KeyCallback> InputHandler::keyDownCallbacks = std::vector<KeyCallback>();
std::vector<KeyCallback> InputHandler::keyUpCallbacks = std::vector<KeyCallback>();
std::vector<MouseButtonCallback> InputHandler::mouseButtonCallbacks = std::vector<MouseButtonCallback>();
std::vector<MouseMotionCallback> InputHandler::mouseMotionCallbacks = std::vector<MouseMotionCallback>();
std::vector<MouseWheelCallback> InputHandler::mouseWheelCallbacks = std::vector<MouseWheelCallback>();
std::vector<QuitEventCallback> InputHandler::quitEventCallbacks = std::vector<QuitEventCallback>();
std::vector<WindowEventCallback> InputHandler::windowEventCallbacks = std::vector<WindowEventCallback>();

void InputHandler::Initialize() {
    InputHandler::logger = spdlog::get("InputHandler");
    InputHandler::logger->trace("InputHandler::Initialize()");
    InputHandler::logger->trace("InputHandler::Initialize()~");
}

void InputHandler::Destroy() {
    InputHandler::logger->trace("InputHandler::Destroy()");
    InputHandler::logger->trace("InputHandler::Destroy()~");
}

void InputHandler::CheckInputs() {
    // todo too many log lines
    // InputHandler::logger->trace("InputHandler::CheckInputs()");
    while (SDL_PollEvent(&InputHandler::sdlEvent) != 0) {
        if (InputHandler::quitEventCallbacks.size() && InputHandler::sdlEvent.type == SDL_QUIT) {
            for (auto callback : InputHandler::quitEventCallbacks) callback();
        } else if (InputHandler::windowEventCallbacks.size() && InputHandler::sdlEvent.type == SDL_WINDOWEVENT) {
            for (auto callback : InputHandler::windowEventCallbacks) callback(InputHandler::sdlEvent.window);
        } else if (InputHandler::keyDownCallbacks.size() && InputHandler::sdlEvent.type == SDL_KEYDOWN) {
            for (auto callback : InputHandler::keyDownCallbacks) callback(InputHandler::sdlEvent.key);
        } else if (InputHandler::keyUpCallbacks.size() && InputHandler::sdlEvent.type == SDL_KEYUP) {
            for (auto callback : InputHandler::keyUpCallbacks) callback(InputHandler::sdlEvent.key);
        } else if (InputHandler::mouseWheelCallbacks.size() && InputHandler::sdlEvent.type == SDL_MOUSEWHEEL) {
            for (auto callback : InputHandler::mouseWheelCallbacks) callback(InputHandler::sdlEvent.wheel);
        } else if (InputHandler::mouseButtonCallbacks.size() &&
                   (InputHandler::sdlEvent.type == SDL_MOUSEBUTTONDOWN || InputHandler::sdlEvent.type == SDL_MOUSEBUTTONUP)) {
            for (auto callback : InputHandler::mouseButtonCallbacks) callback(InputHandler::sdlEvent.button);
        } else if (InputHandler::mouseMotionCallbacks.size() &&
                   (InputHandler::sdlEvent.type == SDL_MOUSEBUTTONDOWN || InputHandler::sdlEvent.type == SDL_MOUSEMOTION)) {
            for (auto callback : InputHandler::mouseMotionCallbacks) callback(InputHandler::sdlEvent.motion);
        }
    }
    // todo too many log lines
    // InputHandler::logger->trace("InputHandler::CheckInputs()~");
}

void InputHandler::RegisterKeyDownEvent(KeyCallback callback) {
    InputHandler::logger->trace("InputHandler::RegisterKeyDownEvent(KeyCallback callback)");
    if (callback) InputHandler::keyDownCallbacks.push_back(callback);
    InputHandler::logger->trace("InputHandler::RegisterKeyDownEvent()~");
}

void InputHandler::RegisterKeyUpEvent(KeyCallback callback) {
    InputHandler::logger->trace("InputHandler::RegisterKeyUpEvent(KeyCallback callback)");
    if (callback) InputHandler::keyUpCallbacks.push_back(callback);
    InputHandler::logger->trace("InputHandler::RegisterKeyUpEvent()~");
}

void InputHandler::RegisterMouseButtonEvent(MouseButtonCallback callback) {
    InputHandler::logger->trace("InputHandler::RegisterMouseButtonEvent(MouseButtonCallback callback)");
    if (callback) InputHandler::mouseButtonCallbacks.push_back(callback);
    InputHandler::logger->trace("InputHandler::RegisterMouseButtonEvent()~");
}

void InputHandler::RegisterMouseMotionEvent(MouseMotionCallback callback) {
    InputHandler::logger->trace("InputHandler::RegisterMouseMotionEvent(MouseMotionCallback callback)");
    if (callback) InputHandler::mouseMotionCallbacks.push_back(callback);
    InputHandler::logger->trace("InputHandler::RegisterMouseMotionEvent()~");
}

void InputHandler::RegisterMouseWheelEvent(MouseWheelCallback callback) {
    InputHandler::logger->trace("InputHandler::RegisterMouseWheelEvent(MouseWheelCallback callback)");
    if (callback) InputHandler::mouseWheelCallbacks.push_back(callback);
    InputHandler::logger->trace("InputHandler::RegisterMouseWheelEvent()~");
}

void InputHandler::RegisterQuitEvent(QuitEventCallback callback) {
    InputHandler::logger->trace("InputHandler::RegisterQuitEvent(QuitEventCallback callback)");
    if (callback) InputHandler::quitEventCallbacks.push_back(callback);
    InputHandler::logger->trace("InputHandler::RegisterQuitEvent()~");
}

void InputHandler::RegisterWindowEvent(WindowEventCallback callback) {
    InputHandler::logger->trace("InputHandler::RegisterWindowEvent(WindowEventCallback callback)");
    if (callback) InputHandler::windowEventCallbacks.push_back(callback);
    InputHandler::logger->trace("InputHandler::RegisterWindowEvent()~");
}
}  // namespace SFG
