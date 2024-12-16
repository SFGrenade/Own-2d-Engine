#ifndef _GLOBALS_SDLINCLUDE_H_
#define _GLOBALS_SDLINCLUDE_H_

#include <string>

// Including SDL headers
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

std::string SDL_EventType_to_string( SDL_EventType eventType );

std::string SDL_WindowFlags_to_string( SDL_WindowFlags flags );
std::string SDL_WindowEventID_to_string( SDL_WindowEventID flags );
std::string SDL_DisplayEventID_to_string( SDL_DisplayEventID flags );
std::string SDL_DisplayOrientation_to_string( SDL_DisplayOrientation flags );
std::string SDL_FlashOperation_to_string( SDL_FlashOperation flags );
std::string SDL_GLattr_to_string( SDL_GLattr flags );
std::string SDL_GLprofile_to_string( SDL_GLprofile flags );
std::string SDL_GLcontextFlag_to_string( SDL_GLcontextFlag flags );
std::string SDL_GLcontextReleaseFlag_to_string( SDL_GLcontextReleaseFlag flags );
std::string SDL_GLContextResetNotification_to_string( SDL_GLContextResetNotification flags );
std::string SDL_HitTestResult_to_string( SDL_HitTestResult flags );

#endif /* _GLOBALS_SDLINCLUDE_H_ */
