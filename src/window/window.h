#pragma once

// Including our headers
#include "../globals.h"

#include <string>

#include "../graphicshandler/graphicshandler.h"

namespace SFG {
    class Window
    {
    private:
        static int xPos;
        static int yPos;
        static int width;
        static int height;
        static std::string windowTitle;
        static SDL_Window* window;
        static GraphicsHandler* graphicsHandler;
    public:
        static void SetSize(int width, int height);
        static void Initialize();
        static void Destroy();

        static bool InitializeSDL();
        static bool InitializeWindow();
        static bool ShowWindow();
        static SDL_Window* GetSdlWindow();
        static GraphicsHandler* GetGraphicsHandler();
    };
}

