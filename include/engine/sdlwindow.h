#ifndef _ENGINE_SDLWINDOW_H_
#define _ENGINE_SDLWINDOW_H_

#include <mutex>
#include <queue>

#include "_globals/sdlInclude.h"
#include "_globals/spdlogInclude.h"


namespace SFG {
namespace Engine {

class SdlEngine;

class SdlWindow {
  public:
  SdlWindow( SdlEngine* sdlEngine );
  ~SdlWindow();

  uint32_t initialize_sdl_window();
  void add_input( SDL_Event const& e );
  void run_input_loop();

  std::string get_title() const;
  void set_title( std::string const& new_title );
  uint32_t get_x() const;
  void set_x( uint32_t new_x );
  uint32_t get_y() const;
  void set_y( uint32_t new_y );
  uint32_t get_width() const;
  void set_width( uint32_t new_width );
  uint32_t get_height() const;
  void set_height( uint32_t new_height );
  SDL_WindowFlags get_flags();
  void set_flags( SDL_WindowFlags new_flags );

  private:
  spdlogger logger_;
  SdlEngine* sdlEngine_;
  std::mutex sdlInputQueueMutex_;
  std::queue< SDL_Event > sdlInputQueue_;
  SDL_Window* sdlWindow_;
  std::string title_;
  uint32_t x_;
  uint32_t y_;
  uint32_t width_;
  uint32_t height_;
  SDL_WindowFlags flags_;
};

}  // namespace Engine
}  // namespace SFG

#endif /* _ENGINE_SDLWINDOW_H_ */
