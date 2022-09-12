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
	    static SDL_Event sdlEvent;
        static std::vector<KeyCallback> keyDownCallbacks;
        static std::vector<KeyCallback> keyUpCallbacks;
        static std::vector<MouseButtonCallback> mouseButtonCallbacks;
        static std::vector<MouseMotionCallback> mouseMotionCallbacks;
        static std::vector<MouseWheelCallback> mouseWheelCallbacks;
        static std::vector<QuitEventCallback> quitEventCallbacks;
        static std::vector<WindowEventCallback> windowEventCallbacks;
    public:
        static void Initialize();
        static void Destroy();

        static void CheckInputs();
        static void RegisterKeyDownEvent(KeyCallback callback);
        static void RegisterKeyUpEvent(KeyCallback callback);
        static void RegisterMouseButtonEvent(MouseButtonCallback callback);
        static void RegisterMouseMotionEvent(MouseMotionCallback callback);
        static void RegisterMouseWheelEvent(MouseWheelCallback callback);
        static void RegisterQuitEvent(QuitEventCallback callback);
        static void RegisterWindowEvent(WindowEventCallback callback);
    };
}

