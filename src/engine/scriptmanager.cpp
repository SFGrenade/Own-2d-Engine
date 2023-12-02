#include "engine/scriptmanager.h"

#include <thread>

#include "engine/script.h"
#include "engine/sdlwindow.h"
#include "engine/sdlwindowrenderer.h"


SFG::Engine::ScriptManager::ScriptManager( SFG::Engine::SdlWindow* sdlWindow )
    : logger_( spdlog::get( "Engine_ScriptManager" ) ), sdlWindow_( sdlWindow ), scripts_(), scriptsMutex_() {
  this->logger_->trace( fmt::runtime( "ScriptManager( sdlWindow = {:p} )" ), static_cast< void* >( sdlWindow ) );

  this->logger_->trace( fmt::runtime( "ScriptManager()~" ) );
}

SFG::Engine::ScriptManager::~ScriptManager() {
  this->logger_->trace( fmt::runtime( "~ScriptManager()" ) );

  this->scriptsMutex_.lock();
  for( SFG::Engine::Script* script : this->scripts_ ) {
    script->end();
    delete script;
  }
  this->scripts_.clear();
  this->scriptsMutex_.unlock();

  this->logger_->trace( fmt::runtime( "~ScriptManager()~" ) );
}

void SFG::Engine::ScriptManager::frame_update( SDL_Renderer* renderer ) {
  // this->logger_->trace( fmt::runtime( "frame_update( renderer = {:p} )" ), static_cast< void const* >( renderer ) );

  this->scriptsMutex_.lock();
  for( SFG::Engine::Script* script : this->scripts_ ) {
    script->frame_update( renderer );
  }
  this->scriptsMutex_.unlock();

  // this->logger_->trace( fmt::runtime( "frame_update()~" ) );
}

void SFG::Engine::ScriptManager::input_update( SDL_Event const& input ) {
  // this->logger_->trace( fmt::runtime( "input_update( input = {:s} )" ), SDL_EventType_to_string( static_cast< SDL_EventType >( input.type ) ) );

  this->scriptsMutex_.lock();
  for( SFG::Engine::Script* script : this->scripts_ ) {
    script->input_update( input );
  }
  this->scriptsMutex_.unlock();

  // this->logger_->trace( fmt::runtime( "input_update()~" ) );
}

void SFG::Engine::ScriptManager::logic_update( std::chrono::secondsLongDouble const& deltaTime ) {
  // this->logger_->trace( fmt::runtime( "logic_update( deltaTime = {:.8F} )" ), deltaTime.count() );

  this->scriptsMutex_.lock();
  for( SFG::Engine::Script* script : this->scripts_ ) {
    script->logic_update( deltaTime );
  }
  this->scriptsMutex_.unlock();

  // this->logger_->trace( fmt::runtime( "logic_update()~" ) );
}

void SFG::Engine::ScriptManager::network_update() {
  // this->logger_->trace( fmt::runtime( "network_update()" ) );

  this->scriptsMutex_.lock();
  for( SFG::Engine::Script* script : this->scripts_ ) {
    script->network_update();
  }
  this->scriptsMutex_.unlock();

  // this->logger_->trace( fmt::runtime( "network_update()~" ) );
}

template < class T >
T* SFG::Engine::ScriptManager::add_script() {
  static_assert( std::is_base_of< SFG::Engine::Script, T >::value, "Class need to be inherited from `SFG::Engine::Script`!" );
  this->logger_->trace( fmt::runtime( "add_script()" ) );

  T* script = new T();
  script->set_sdlWindow( this->sdlWindow_ );
  script->set_scriptManager( this );
  script->set_scriptList( &( this->scripts_ ) );
  this->scripts_.push_back( script );
  script->start();

  this->logger_->trace( fmt::runtime( "add_script()~" ) );
  return script;
}

// ################################################################# Templated Stuff #################################################################

#include <content/scripts/debuginfo.h>
#include <content/scripts/logscript.h>
#include <content/scripts/player.h>
#include <content/scripts/wall.h>

template SFG::Content::DebugInfo* SFG::Engine::ScriptManager::add_script< SFG::Content::DebugInfo >();
template SFG::Content::LogScript* SFG::Engine::ScriptManager::add_script< SFG::Content::LogScript >();
template SFG::Content::Player* SFG::Engine::ScriptManager::add_script< SFG::Content::Player >();
template SFG::Content::Wall* SFG::Engine::ScriptManager::add_script< SFG::Content::Wall >();
