#pragma once

// Including our headers
#include "../globals.h"

#include <functional>
#include <vector>

namespace SFG {
    typedef std::function<void(SDL_KeyboardEvent& key)> KeyCallback;
    typedef std::function<void(SDL_MouseButtonEvent& button)> MouseButtonCallback;
    typedef std::function<void(SDL_MouseMotionEvent& motion)> MouseMotionCallback;
    typedef std::function<void(SDL_MouseWheelEvent& wheel)> MouseWheelCallback;
    typedef std::function<void()> QuitEventCallback;
    typedef std::function<void(SDL_WindowEvent& window)> WindowEventCallback;

    class InputHandler
    {
    private:
        static InputHandler* instance;
	    SDL_Event sdlEvent;
        std::vector<KeyCallback> keyDownCallbacks;
        std::vector<KeyCallback> keyUpCallbacks;
        std::vector<MouseButtonCallback> mouseButtonCallbacks;
        std::vector<MouseMotionCallback> mouseMotionCallbacks;
        std::vector<MouseWheelCallback> mouseWheelCallbacks;
        std::vector<QuitEventCallback> quitEventCallbacks;
        std::vector<WindowEventCallback> windowEventCallbacks;
        InputHandler(/* args */);
    public:
        ~InputHandler();
        static InputHandler& GetInstance();

        void CheckInputs();
        void RegisterKeyDownEvent(KeyCallback callback);
        void RegisterKeyUpEvent(KeyCallback callback);
        void RegisterMouseButtonEvent(MouseButtonCallback callback);
        void RegisterMouseMotionEvent(MouseMotionCallback callback);
        void RegisterMouseWheelEvent(MouseWheelCallback callback);
        void RegisterQuitEvent(QuitEventCallback callback);
        void RegisterWindowEvent(WindowEventCallback callback);
    };
}
