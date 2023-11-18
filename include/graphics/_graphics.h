#pragma once

#include <functional>
#include <thread>
#include <vector>
#include <zmqPb/pubsub.hpp>

// Including our headers
#include "_globals/spdlogInclude.h"
#include "inProcMessages.pb.h"

namespace SFG {
class Graphics {
  public:
  Graphics();
  ~Graphics();

  void start();
  void join();

  void add_Stop_Thread_callback( std::function< void( SFG::Proto::InProc::Stop_Thread_Request const& ) > callback );
  void add_Get_Config_callback( std::function< void( SFG::Proto::InProc::Get_Config_Reply const& ) > callback );
  void add_Set_Window_Size_callback( std::function< void( SFG::Proto::InProc::Set_Window_Size_Request const& ) > callback );
  void add_Initialize_SDL_And_Window_callback( std::function< void( SFG::Proto::InProc::Initialize_SDL_And_Window_Request const& ) > callback );
  void add_List_Renderers_callback( std::function< void( SFG::Proto::InProc::List_Renderers_Request const& ) > callback );
  void add_Set_Renderer_callback( std::function< void( SFG::Proto::InProc::Set_Renderer_Request const& ) > callback );
  void add_Show_Window_And_Start_Draw_callback( std::function< void( SFG::Proto::InProc::Show_Window_And_Start_Draw_Request const& ) > callback );
  void add_Stop_Draw_callback( std::function< void( SFG::Proto::InProc::Stop_Draw_Request const& ) > callback );
  void add_Update_Performance_Information_callback( std::function< void( SFG::Proto::InProc::Update_Performance_Information_Request const& ) > callback );
  void add_Get_Performance_Counters_callback( std::function< void( SFG::Proto::InProc::Get_Performance_Counters_Request const& ) > callback );

  private:
  spdlogger logger_;
  ZmqPb::PubSub network_Graphics_Send_;
  ZmqPb::PubSub network_Graphics_Receive_;
  ZmqPb::PubSub network_Input_Receive_;
  ZmqPb::PubSub network_Logic_Receive_;
  ZmqPb::PubSub network_Network_Receive_;
  std::unique_ptr< std::thread > workerThread_;
  bool workerThreadIsRunning_;

  private:
  uint64_t performanceLoops_;
  std::vector< std::function< void( SFG::Proto::InProc::Stop_Thread_Request const& ) > > stop_thread_callbacks_;
  std::vector< std::function< void( SFG::Proto::InProc::Get_Config_Reply const& ) > > get_config_callbacks_;
  std::vector< std::function< void( SFG::Proto::InProc::Set_Window_Size_Request const& ) > > set_window_size_callbacks_;
  std::vector< std::function< void( SFG::Proto::InProc::Initialize_SDL_And_Window_Request const& ) > > initialize_sdl_and_window_callbacks_;
  std::vector< std::function< void( SFG::Proto::InProc::List_Renderers_Request const& ) > > list_renderers_callbacks_;
  std::vector< std::function< void( SFG::Proto::InProc::Set_Renderer_Request const& ) > > set_renderer_callbacks_;
  std::vector< std::function< void( SFG::Proto::InProc::Show_Window_And_Start_Draw_Request const& ) > > show_window_and_start_draw_callbacks_;
  std::vector< std::function< void( SFG::Proto::InProc::Stop_Draw_Request const& ) > > stop_draw_callbacks_;
  std::vector< std::function< void( SFG::Proto::InProc::Update_Performance_Information_Request const& ) > > update_performance_information_callbacks_;
  std::vector< std::function< void( SFG::Proto::InProc::Get_Performance_Counters_Request const& ) > > get_performance_counters_callbacks_;
};
}  // namespace SFG
