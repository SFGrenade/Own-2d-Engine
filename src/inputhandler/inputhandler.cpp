#include "inputhandler.h"

// Include our overall Objects
#include "../globals.h"

namespace SFG {
    InputHandler* InputHandler::instance = nullptr;

    InputHandler::InputHandler()
        : sdlEvent()
        , keyDownCallbacks()
        , keyUpCallbacks()
        , mouseButtonCallbacks()
        , mouseMotionCallbacks()
        , mouseWheelCallbacks()
        , quitEventCallbacks()
        , windowEventCallbacks() {
        spdlog::trace("InputHandler::InputHandler()");
        spdlog::trace("InputHandler::InputHandler()~");
    }

    InputHandler::~InputHandler() {
        spdlog::trace("InputHandler::~InputHandler()");
        spdlog::trace("InputHandler::~InputHandler()~");
    }

    InputHandler* InputHandler::GetInstance() {
        spdlog::trace("InputHandler::GetInstance()");
        if (!InputHandler::instance) InputHandler::instance = new InputHandler();
        spdlog::trace("InputHandler::GetInstance()~");
        return InputHandler::instance;
    }

    void InputHandler::CheckInputs() {
        //spdlog::trace("InputHandler::CheckInputs()"); // todo too many log lines
        while (SDL_PollEvent(&sdlEvent) != 0) {
            if ( quitEventCallbacks.size() && sdlEvent.type == SDL_QUIT ) {
                for ( auto callback : quitEventCallbacks ) callback();
            }
            else if ( windowEventCallbacks.size() && sdlEvent.type == SDL_WINDOWEVENT ) {
                for ( auto callback : windowEventCallbacks ) callback(sdlEvent.window);
            }
            else if ( keyDownCallbacks.size() && sdlEvent.type == SDL_KEYDOWN ) {
                for ( auto callback : keyDownCallbacks ) callback(sdlEvent.key);
            }
            else if ( keyUpCallbacks.size() && sdlEvent.type == SDL_KEYUP ) {
                for ( auto callback : keyUpCallbacks ) callback(sdlEvent.key);
            }
            else if ( mouseWheelCallbacks.size() && sdlEvent.type == SDL_MOUSEWHEEL ) {
                for ( auto callback : mouseWheelCallbacks ) callback(sdlEvent.wheel);
            }
            else if ( mouseButtonCallbacks.size() && (sdlEvent.type == SDL_MOUSEBUTTONDOWN || sdlEvent.type == SDL_MOUSEBUTTONUP) ) {
                for ( auto callback : mouseButtonCallbacks ) callback(sdlEvent.button);
            }
            else if ( mouseMotionCallbacks.size() && (sdlEvent.type == SDL_MOUSEBUTTONDOWN || sdlEvent.type == SDL_MOUSEMOTION) ) {
                for ( auto callback : mouseMotionCallbacks ) callback(sdlEvent.motion);
            }
        }
        //spdlog::trace("InputHandler::CheckInputs()~"); // todo too many log lines
    }

    void InputHandler::RegisterKeyDownEvent(KeyCallback callback) {
        spdlog::trace("InputHandler::RegisterKeyDownEvent(KeyCallback callback)");
        if (callback) keyDownCallbacks.push_back(callback);
        spdlog::trace("InputHandler::RegisterKeyDownEvent()~");
    }

    void InputHandler::RegisterKeyUpEvent(KeyCallback callback) {
        spdlog::trace("InputHandler::RegisterKeyUpEvent(KeyCallback callback)");
        if (callback) keyUpCallbacks.push_back(callback);
        spdlog::trace("InputHandler::RegisterKeyUpEvent()~");
    }

    void InputHandler::RegisterMouseButtonEvent(MouseButtonCallback callback) {
        spdlog::trace("InputHandler::RegisterMouseButtonEvent(MouseButtonCallback callback)");
        if (callback) mouseButtonCallbacks.push_back(callback);
        spdlog::trace("InputHandler::RegisterMouseButtonEvent()~");
    }

    void InputHandler::RegisterMouseMotionEvent(MouseMotionCallback callback) {
        spdlog::trace("InputHandler::RegisterMouseMotionEvent(MouseMotionCallback callback)");
        if (callback) mouseMotionCallbacks.push_back(callback);
        spdlog::trace("InputHandler::RegisterMouseMotionEvent()~");
    }

    void InputHandler::RegisterMouseWheelEvent(MouseWheelCallback callback) {
        spdlog::trace("InputHandler::RegisterMouseWheelEvent(MouseWheelCallback callback)");
        if (callback) mouseWheelCallbacks.push_back(callback);
        spdlog::trace("InputHandler::RegisterMouseWheelEvent()~");
    }

    void InputHandler::RegisterQuitEvent(QuitEventCallback callback) {
        spdlog::trace("InputHandler::RegisterQuitEvent(QuitEventCallback callback)");
        if (callback) quitEventCallbacks.push_back(callback);
        spdlog::trace("InputHandler::RegisterQuitEvent()~");
    }

    void InputHandler::RegisterWindowEvent(WindowEventCallback callback) {
        spdlog::trace("InputHandler::RegisterWindowEvent(WindowEventCallback callback)");
        if (callback) windowEventCallbacks.push_back(callback);
        spdlog::trace("InputHandler::RegisterWindowEvent()~");
    }
}
