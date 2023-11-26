#ifndef _ENGINE_SDLWINDOW_H_
#define _ENGINE_SDLWINDOW_H_

#include <mutex>
#include <queue>
#include <thread>

#include "_globals/sdlInclude.h"
#include "_globals/spdlogInclude.h"


namespace SFG {
namespace Engine {

class SdlEngine;
class PerformanceController;
class SdlWindowRenderer;
class LogicController;

class SdlWindow {
  public:
  SdlWindow( SFG::Engine::SdlEngine* sdlEngine );
  ~SdlWindow();

  void initialize_sdl_window();
  SFG::Engine::SdlWindowRenderer* initialize_window_renderer( std::string const& renderer, SDL_RendererFlags flags );
  SFG::Engine::LogicController* initialize_logic_controller();
  void add_input( SDL_Event const& e );
  void run_input_loop();

  SDL_Window* get_sdl_window() const;
  uint32_t get_sdl_window_id() const;
  SFG::Engine::PerformanceController* get_performance_controller() const;
  SFG::Engine::SdlWindowRenderer* get_window_renderer() const;
  SFG::Engine::LogicController* get_logic_controller() const;

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
  __declspec( align( 64 ) ) spdlogger logger_;

  __declspec( align( 64 ) ) SFG::Engine::SdlEngine* sdlEngine_;
  __declspec( align( 64 ) ) std::queue< SDL_Event > sdlInputQueue_;
  __declspec( align( 64 ) ) std::mutex sdlInputQueueMutex_;

  __declspec( align( 64 ) ) SDL_Window* sdlWindow_;
  __declspec( align( 64 ) ) uint32_t sdlWindowId_;
  __declspec( align( 64 ) ) SFG::Engine::PerformanceController* performanceController_;
  __declspec( align( 64 ) ) SFG::Engine::SdlWindowRenderer* sdlRenderer_;
  __declspec( align( 64 ) ) std::thread sdlRendererThread_;
  __declspec( align( 64 ) ) SFG::Engine::LogicController* logicController_;
  __declspec( align( 64 ) ) std::thread logicControllerThread_;

  __declspec( align( 64 ) ) std::string title_;
  __declspec( align( 64 ) ) uint32_t x_;
  __declspec( align( 64 ) ) uint32_t y_;
  __declspec( align( 64 ) ) uint32_t width_;
  __declspec( align( 64 ) ) uint32_t height_;
  __declspec( align( 64 ) ) SDL_WindowFlags flags_;
};

}  // namespace Engine
}  // namespace SFG

#endif /* _ENGINE_SDLWINDOW_H_ */
