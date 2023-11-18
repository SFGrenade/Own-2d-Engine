#include "graphics/_graphics.h"

#include <string>
#include <thread>

#include "_globals/misc.h"
#include "_globals/zmq.h"

namespace SFG {
Graphics::Graphics()
    : logger_( spdlog::get( "TSrv" ) ),
      network_Graphics_Send_( "inproc://graphics", true, &global_inproc_context_ ),
      network_Graphics_Receive_( "inproc://graphics", false, &global_inproc_context_ ),
      network_Input_Receive_( "inproc://input", false, &global_inproc_context_ ),
      network_Logic_Receive_( "inproc://logic", false, &global_inproc_context_ ),
      network_Network_Receive_( "inproc://network", false, &global_inproc_context_ ),
      workerThread_( nullptr ),
      workerThreadIsRunning_( false ),
      performanceLoops_( 0 ),
      stop_thread_callbacks_(),
      get_config_callbacks_(),
      set_window_size_callbacks_(),
      initialize_sdl_and_window_callbacks_(),
      list_renderers_callbacks_(),
      set_renderer_callbacks_(),
      show_window_and_start_draw_callbacks_(),
      stop_draw_callbacks_(),
      update_performance_information_callbacks_(),
      get_performance_counters_callbacks_() {
  this->logger_->trace( fmt::runtime( "[thread {:s}] Graphics()" ), getThreadId() );

  add_Get_Performance_Counters_callback( [this]( SFG::Proto::InProc::Get_Performance_Counters_Request const& ) {
    this->logger_->trace( fmt::runtime( "[thread {:s}] Get_Performance_Counters_callback()" ), getThreadId() );

    SFG::Proto::InProc::Get_Performance_Counters_Reply* repMsg = new SFG::Proto::InProc::Get_Performance_Counters_Reply();
    repMsg->set_counter_graphics( this->performanceLoops_ );
    network_Graphics_Send_.sendMessage( repMsg );
    this->performanceLoops_ = 0;

    this->logger_->trace( fmt::runtime( "[thread {:s}] Get_Performance_Counters_callback()~" ), getThreadId() );
  } );
  add_Stop_Thread_callback( [this]( SFG::Proto::InProc::Stop_Thread_Request const& ) {
    this->logger_->trace( fmt::runtime( "[thread {:s}] Stop_Thread_callback()" ), getThreadId() );

    this->workerThreadIsRunning_ = false;

    this->logger_->trace( fmt::runtime( "[thread {:s}] Stop_Thread_callback()~" ), getThreadId() );
  } );

  network_Input_Receive_.subscribe( new SFG::Proto::InProc::Stop_Thread_Request(), [this]( google::protobuf::Message const& message ) {
    for( auto callback : this->stop_thread_callbacks_ )
      callback( static_cast< SFG::Proto::InProc::Stop_Thread_Request const& >( message ) );
  } );
  network_Input_Receive_.subscribe( new SFG::Proto::InProc::Get_Config_Reply(), [this]( google::protobuf::Message const& message ) {
    for( auto callback : this->get_config_callbacks_ )
      callback( static_cast< SFG::Proto::InProc::Get_Config_Reply const& >( message ) );
  } );
  network_Input_Receive_.subscribe( new SFG::Proto::InProc::Set_Window_Size_Request(), [this]( google::protobuf::Message const& message ) {
    for( auto callback : this->set_window_size_callbacks_ )
      callback( static_cast< SFG::Proto::InProc::Set_Window_Size_Request const& >( message ) );
  } );
  network_Input_Receive_.subscribe( new SFG::Proto::InProc::Initialize_SDL_And_Window_Request(), [this]( google::protobuf::Message const& message ) {
    for( auto callback : this->initialize_sdl_and_window_callbacks_ )
      callback( static_cast< SFG::Proto::InProc::Initialize_SDL_And_Window_Request const& >( message ) );
  } );
  network_Input_Receive_.subscribe( new SFG::Proto::InProc::List_Renderers_Request(), [this]( google::protobuf::Message const& message ) {
    for( auto callback : this->list_renderers_callbacks_ )
      callback( static_cast< SFG::Proto::InProc::List_Renderers_Request const& >( message ) );
  } );
  network_Input_Receive_.subscribe( new SFG::Proto::InProc::Set_Renderer_Request(), [this]( google::protobuf::Message const& message ) {
    for( auto callback : this->set_renderer_callbacks_ )
      callback( static_cast< SFG::Proto::InProc::Set_Renderer_Request const& >( message ) );
  } );
  network_Input_Receive_.subscribe( new SFG::Proto::InProc::Show_Window_And_Start_Draw_Request(), [this]( google::protobuf::Message const& message ) {
    for( auto callback : this->show_window_and_start_draw_callbacks_ )
      callback( static_cast< SFG::Proto::InProc::Show_Window_And_Start_Draw_Request const& >( message ) );
  } );
  network_Input_Receive_.subscribe( new SFG::Proto::InProc::Stop_Draw_Request(), [this]( google::protobuf::Message const& message ) {
    for( auto callback : this->stop_draw_callbacks_ )
      callback( static_cast< SFG::Proto::InProc::Stop_Draw_Request const& >( message ) );
  } );
  network_Logic_Receive_.subscribe( new SFG::Proto::InProc::Update_Performance_Information_Request(), [this]( google::protobuf::Message const& message ) {
    for( auto callback : this->update_performance_information_callbacks_ )
      callback( static_cast< SFG::Proto::InProc::Update_Performance_Information_Request const& >( message ) );
  } );
  network_Logic_Receive_.subscribe( new SFG::Proto::InProc::Get_Performance_Counters_Request(), [this]( google::protobuf::Message const& message ) {
    for( auto callback : this->get_performance_counters_callbacks_ )
      callback( static_cast< SFG::Proto::InProc::Get_Performance_Counters_Request const& >( message ) );
  } );

  this->logger_->trace( fmt::runtime( "[thread {:s}] Graphics()~" ), getThreadId() );
}

Graphics::~Graphics() {
  this->logger_->trace( fmt::runtime( "[thread {:s}] ~Graphics()" ), getThreadId() );

  this->logger_->trace( fmt::runtime( "[thread {:s}] ~Graphics()~" ), getThreadId() );
}

#define tryCatchZmqpbRun( x )                                                                                      \
  try {                                                                                                            \
    ( x ).run();                                                                                                   \
  } catch( std::exception const& e ) {                                                                             \
    this->logger_->error( fmt::runtime( "[thread {:s}] thread - network error: {:s}" ), getThreadId(), e.what() ); \
  }

void Graphics::start() {
  this->logger_->trace( fmt::runtime( "[thread {:s}] start()" ), getThreadId() );

  this->workerThreadIsRunning_ = true;
  this->workerThread_ = std::make_unique< std::thread >( [this]() {
    this->logger_->trace( fmt::runtime( "[thread {:s}] thread()" ), getThreadId() );

    while( this->workerThreadIsRunning_ ) {  // inproc networking
      // inproc networking
      tryCatchZmqpbRun( this->network_Graphics_Send_ );
      tryCatchZmqpbRun( this->network_Graphics_Receive_ );
      tryCatchZmqpbRun( this->network_Input_Receive_ );
      tryCatchZmqpbRun( this->network_Logic_Receive_ );
      tryCatchZmqpbRun( this->network_Network_Receive_ );
      ++performanceLoops_;
    }

    this->logger_->trace( fmt::runtime( "[thread {:s}] thread()~" ), getThreadId() );
  } );

  this->logger_->trace( fmt::runtime( "[thread {:s}] start()~" ), getThreadId() );
}

void Graphics::join() {
  this->logger_->trace( fmt::runtime( "[thread {:s}] join()" ), getThreadId() );

  this->workerThread_->join();

  this->logger_->trace( fmt::runtime( "[thread {:s}] join()~" ), getThreadId() );
}

void Graphics::add_Stop_Thread_callback( std::function< void( SFG::Proto::InProc::Stop_Thread_Request const& ) > callback ) {
  this->logger_->trace( fmt::runtime( "[thread {:s}] add_Stop_Thread_callback( callback )" ), getThreadId() );

  if( callback )
    stop_thread_callbacks_.push_back( callback );

  this->logger_->trace( fmt::runtime( "[thread {:s}] add_Stop_Thread_callback()~" ), getThreadId() );
}

void Graphics::add_Get_Config_callback( std::function< void( SFG::Proto::InProc::Get_Config_Reply const& ) > callback ) {
  this->logger_->trace( fmt::runtime( "[thread {:s}] add_Get_Config_callback( callback )" ), getThreadId() );

  if( callback )
    get_config_callbacks_.push_back( callback );

  this->logger_->trace( fmt::runtime( "[thread {:s}] add_Get_Config_callback()~" ), getThreadId() );
}

void Graphics::add_Set_Window_Size_callback( std::function< void( SFG::Proto::InProc::Set_Window_Size_Request const& ) > callback ) {
  this->logger_->trace( fmt::runtime( "[thread {:s}] add_Set_Window_Size_callback( callback )" ), getThreadId() );

  if( callback )
    set_window_size_callbacks_.push_back( callback );

  this->logger_->trace( fmt::runtime( "[thread {:s}] add_Set_Window_Size_callback()~" ), getThreadId() );
}

void Graphics::add_Initialize_SDL_And_Window_callback( std::function< void( SFG::Proto::InProc::Initialize_SDL_And_Window_Request const& ) > callback ) {
  this->logger_->trace( fmt::runtime( "[thread {:s}] add_Initialize_SDL_And_Window_callback( callback )" ), getThreadId() );

  if( callback )
    initialize_sdl_and_window_callbacks_.push_back( callback );

  this->logger_->trace( fmt::runtime( "[thread {:s}] add_Initialize_SDL_And_Window_callback()~" ), getThreadId() );
}

void Graphics::add_List_Renderers_callback( std::function< void( SFG::Proto::InProc::List_Renderers_Request const& ) > callback ) {
  this->logger_->trace( fmt::runtime( "[thread {:s}] add_List_Renderers_callback( callback )" ), getThreadId() );

  if( callback )
    list_renderers_callbacks_.push_back( callback );

  this->logger_->trace( fmt::runtime( "[thread {:s}] add_List_Renderers_callback()~" ), getThreadId() );
}

void Graphics::add_Set_Renderer_callback( std::function< void( SFG::Proto::InProc::Set_Renderer_Request const& ) > callback ) {
  this->logger_->trace( fmt::runtime( "[thread {:s}] add_Set_Renderer_callback( callback )" ), getThreadId() );

  if( callback )
    set_renderer_callbacks_.push_back( callback );

  this->logger_->trace( fmt::runtime( "[thread {:s}] add_Set_Renderer_callback()~" ), getThreadId() );
}

void Graphics::add_Show_Window_And_Start_Draw_callback( std::function< void( SFG::Proto::InProc::Show_Window_And_Start_Draw_Request const& ) > callback ) {
  this->logger_->trace( fmt::runtime( "[thread {:s}] add_Show_Window_And_Start_Draw_callback( callback )" ), getThreadId() );

  if( callback )
    show_window_and_start_draw_callbacks_.push_back( callback );

  this->logger_->trace( fmt::runtime( "[thread {:s}] add_Show_Window_And_Start_Draw_callback()~" ), getThreadId() );
}

void Graphics::add_Stop_Draw_callback( std::function< void( SFG::Proto::InProc::Stop_Draw_Request const& ) > callback ) {
  this->logger_->trace( fmt::runtime( "[thread {:s}] add_Stop_Draw_callback( callback )" ), getThreadId() );

  if( callback )
    stop_draw_callbacks_.push_back( callback );

  this->logger_->trace( fmt::runtime( "[thread {:s}] add_Stop_Draw_callback()~" ), getThreadId() );
}

void Graphics::add_Update_Performance_Information_callback(
    std::function< void( SFG::Proto::InProc::Update_Performance_Information_Request const& ) > callback ) {
  this->logger_->trace( fmt::runtime( "[thread {:s}] add_Update_Performance_Information_callback( callback )" ), getThreadId() );

  if( callback )
    update_performance_information_callbacks_.push_back( callback );

  this->logger_->trace( fmt::runtime( "[thread {:s}] add_Update_Performance_Information_callback()~" ), getThreadId() );
}

void Graphics::add_Get_Performance_Counters_callback( std::function< void( SFG::Proto::InProc::Get_Performance_Counters_Request const& ) > callback ) {
  this->logger_->trace( fmt::runtime( "[thread {:s}] add_Get_Performance_Counters_callback( callback )" ), getThreadId() );

  if( callback )
    get_performance_counters_callbacks_.push_back( callback );

  this->logger_->trace( fmt::runtime( "[thread {:s}] add_Get_Performance_Counters_callback()~" ), getThreadId() );
}
}  // namespace SFG
