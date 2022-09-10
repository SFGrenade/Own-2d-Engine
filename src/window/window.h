#pragma once

// Including our headers
#include "../globals.h"

#include <string>

#include "../graphicshandler/graphicshandler.h"

namespace SFG {
    class Window
    {
    private:
        static Window* instance;
        int xPos;
        int yPos;
        int width;
        int height;
        std::string windowTitle;
        SDL_Window* window;
        GraphicsHandler* graphicsHandler;
        Window();
    public:
        ~Window();
        static Window* GetInstance();

        bool InitializeSDL();
        bool InitializeWindow();
        bool ShowWindow();
        SDL_Window* GetSdlWindow();
        GraphicsHandler* GetGraphicsHandler();
    };
}
