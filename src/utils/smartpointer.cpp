#include "smartpointer.h"

// Including SDL headers
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../fonthandler/fonthandler.h"
#include "../graphicshandler/graphicshandler.h"
#include "../inputhandler/inputhandler.h"
#include "../logichandler/logichandler.h"
#include "../logichandler/timer.h"
#include "../window/window.h"

template class SFG::Utils::SmartPointer<bool>;
template class SFG::Utils::SmartPointer<SDL_Texture>;
template class SFG::Utils::SmartPointer<SFG::FontHandler>;
template class SFG::Utils::SmartPointer<TTF_Font>;
template class SFG::Utils::SmartPointer<SFG::GraphicsHandler>;
template class SFG::Utils::SmartPointer<SDL_Renderer>;
template class SFG::Utils::SmartPointer<SFG::InputHandler>;
template class SFG::Utils::SmartPointer<SFG::LogicHandler>;
template class SFG::Utils::SmartPointer<SFG::Timer>;
template class SFG::Utils::SmartPointer<SFG::Window>;
template class SFG::Utils::SmartPointer<SDL_Window>;
