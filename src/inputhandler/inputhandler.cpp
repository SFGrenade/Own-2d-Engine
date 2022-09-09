#include "inputhandler.h"

// Include our overall Objects
#include "../globals.h"

namespace SFG {
    InputHandler* InputHandler::instance = nullptr;

    InputHandler::InputHandler(/* args */)
        : sdlEvent()
        , keyDownCallbacks()
        , keyUpCallbacks()
        , mouseButtonCallbacks()
        , mouseMotionCallbacks()
        , mouseWheelCallbacks()
        , quitEventCallbacks()
        , windowEventCallbacks() {
    }

    InputHandler::~InputHandler() {
    }

    InputHandler* InputHandler::GetInstance() {
        if (!InputHandler::instance) InputHandler::instance = new InputHandler();
        return InputHandler::instance;
    }

    void InputHandler::CheckInputs() {
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
    }

    void InputHandler::RegisterKeyDownEvent(KeyCallback callback) {
        if (callback == nullptr) return;
        keyDownCallbacks.push_back(callback);
    }

    void InputHandler::RegisterKeyUpEvent(KeyCallback callback) {
        if (callback == nullptr) return;
        keyUpCallbacks.push_back(callback);
    }

    void InputHandler::RegisterMouseButtonEvent(MouseButtonCallback callback) {
        if (callback == nullptr) return;
        mouseButtonCallbacks.push_back(callback);
    }

    void InputHandler::RegisterMouseMotionEvent(MouseMotionCallback callback) {
        if (callback == nullptr) return;
        mouseMotionCallbacks.push_back(callback);
    }

    void InputHandler::RegisterMouseWheelEvent(MouseWheelCallback callback) {
        if (callback == nullptr) return;
        mouseWheelCallbacks.push_back(callback);
    }

    void InputHandler::RegisterQuitEvent(QuitEventCallback callback) {
        if (callback == nullptr) return;
        quitEventCallbacks.push_back(callback);
    }

    void InputHandler::RegisterWindowEvent(WindowEventCallback callback) {
        if (callback == nullptr) return;
        windowEventCallbacks.push_back(callback);
    }
}
