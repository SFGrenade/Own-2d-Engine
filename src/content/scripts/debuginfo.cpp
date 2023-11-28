#include "content/scripts/debuginfo.h"

#include "engine/performancecontroller.h"
#include "engine/sdlwindow.h"


SFG::Content::DebugInfo::DebugInfo( SFG::Engine::SdlWindow* sdlWindow )
    : SFG::Engine::Script( sdlWindow ),
      logger_( spdlog::get( "Content_DebugInfo" ) ),
      rendering_( true ),
      countDown1s_( 50 ),  // fixed_update is called at 50hz
      debugInfoTopLeft_( { "", false, nullptr, SDL_Rect( 0, 0, 0, 0 ) } ),
      debugInfoTopRight_( { "", false, nullptr, SDL_Rect( 0, 0, 0, 0 ) } ),
      debugInfoBottomLeft_( { "", false, nullptr, SDL_Rect( 0, 0, 0, 0 ) } ),
      debugInfoBottomRight_( { "", false, nullptr, SDL_Rect( 0, 0, 0, 0 ) } ),
      sdlFont_( TTF_OpenFont( R"(./Resources/Fonts/NotoSansMono-Regular.ttf)", 18 ) ) {
  if( this->sdlFont_ == nullptr ) {
    this->logger_->error( fmt::runtime( "DebugInfo - Error when TTF_OpenFont: {:s}" ), TTF_GetError() );
  }
}

SFG::Content::DebugInfo::~DebugInfo() {
  if( this->sdlFont_ ) {
    TTF_CloseFont( this->sdlFont_ );
    this->sdlFont_ = nullptr;
  }
  if( this->debugInfoTopLeft_.texture_ ) {
    SDL_DestroyTexture( this->debugInfoTopLeft_.texture_ );
  }
  if( this->debugInfoTopRight_.texture_ ) {
    SDL_DestroyTexture( this->debugInfoTopRight_.texture_ );
  }
  if( this->debugInfoBottomLeft_.texture_ ) {
    SDL_DestroyTexture( this->debugInfoBottomLeft_.texture_ );
  }
  if( this->debugInfoBottomRight_.texture_ ) {
    SDL_DestroyTexture( this->debugInfoBottomRight_.texture_ );
  }
}

void SFG::Content::DebugInfo::start() {
  SFG::Engine::Script::start();
}

void SFG::Content::DebugInfo::frame_update( SDL_Renderer const* renderer ) {
  SFG::Engine::Script::frame_update( renderer );

  if( this->debugInfoTopLeft_.drawNew_ ) {
    this->renderDebugInfo( this->debugInfoTopLeft_, renderer );
    this->debugInfoTopLeft_.textureRect_.x = 0;
    this->debugInfoTopLeft_.textureRect_.y = 0;
  }
  if( this->debugInfoTopRight_.drawNew_ ) {
    this->renderDebugInfo( this->debugInfoTopRight_, renderer );
    this->debugInfoTopRight_.textureRect_.x = this->sdlWindow_->get_width() - this->debugInfoTopRight_.textureRect_.w;
    this->debugInfoTopRight_.textureRect_.y = 0;
  }
  if( this->debugInfoBottomLeft_.drawNew_ ) {
    this->renderDebugInfo( this->debugInfoBottomLeft_, renderer );
    this->debugInfoBottomLeft_.textureRect_.x = 0;
    this->debugInfoBottomLeft_.textureRect_.y = this->sdlWindow_->get_height() - this->debugInfoBottomLeft_.textureRect_.h;
  }
  if( this->debugInfoBottomRight_.drawNew_ ) {
    this->renderDebugInfo( this->debugInfoBottomRight_, renderer );
    this->debugInfoBottomRight_.textureRect_.x = this->sdlWindow_->get_width() - this->debugInfoBottomRight_.textureRect_.w;
    this->debugInfoBottomRight_.textureRect_.y = this->sdlWindow_->get_height() - this->debugInfoBottomRight_.textureRect_.h;
  }

  if( this->rendering_ && this->debugInfoTopLeft_.textureRect_.w > 0 && this->debugInfoTopLeft_.textureRect_.h > 0 && this->debugInfoTopLeft_.texture_ ) {
    if( SDL_RenderCopy( const_cast< SDL_Renderer* >( renderer ), this->debugInfoTopLeft_.texture_, NULL, &( this->debugInfoTopLeft_.textureRect_ ) ) != 0 ) {
      this->logger_->error( fmt::runtime( "frame_update - Error when SDL_RenderCopy: {:s}" ), SDL_GetError() );
    }
  }
  if( this->rendering_ && this->debugInfoTopRight_.textureRect_.w > 0 && this->debugInfoTopRight_.textureRect_.h > 0 && this->debugInfoTopRight_.texture_ ) {
    if( SDL_RenderCopy( const_cast< SDL_Renderer* >( renderer ), this->debugInfoTopRight_.texture_, NULL, &( this->debugInfoTopRight_.textureRect_ ) ) != 0 ) {
      this->logger_->error( fmt::runtime( "frame_update - Error when SDL_RenderCopy: {:s}" ), SDL_GetError() );
    }
  }
  if( this->rendering_ && this->debugInfoBottomLeft_.textureRect_.w > 0 && this->debugInfoBottomLeft_.textureRect_.h > 0
      && this->debugInfoBottomLeft_.texture_ ) {
    if( SDL_RenderCopy( const_cast< SDL_Renderer* >( renderer ), this->debugInfoBottomLeft_.texture_, NULL, &( this->debugInfoBottomLeft_.textureRect_ ) )
        != 0 ) {
      this->logger_->error( fmt::runtime( "frame_update - Error when SDL_RenderCopy: {:s}" ), SDL_GetError() );
    }
  }
  if( this->rendering_ && this->debugInfoBottomRight_.textureRect_.w > 0 && this->debugInfoBottomRight_.textureRect_.h > 0
      && this->debugInfoBottomRight_.texture_ ) {
    if( SDL_RenderCopy( const_cast< SDL_Renderer* >( renderer ), this->debugInfoBottomRight_.texture_, NULL, &( this->debugInfoBottomRight_.textureRect_ ) )
        != 0 ) {
      this->logger_->error( fmt::runtime( "frame_update - Error when SDL_RenderCopy: {:s}" ), SDL_GetError() );
    }
  }
}

void SFG::Content::DebugInfo::input_update( SDL_Event const& input ) {
  SFG::Engine::Script::input_update( input );

  if( input.type == SDL_EventType::SDL_KEYDOWN ) {
    if( input.key.keysym.sym == SDL_KeyCode::SDLK_F10 ) {
      this->rendering_ = !this->rendering_;
    }
  }
}

void SFG::Content::DebugInfo::logic_update( std::chrono::secondsLongDouble const& deltaTime ) {
  SFG::Engine::Script::logic_update( deltaTime );
}

void SFG::Content::DebugInfo::fixed_update() {
  SFG::Engine::Script::fixed_update();

  --( this->countDown1s_ );
  if( this->countDown1s_ <= 0 ) {
    this->set_debugInfo_topLeft( fmt::format( fmt::runtime( "{:.1F} fps" ), this->sdlWindow_->get_performance_controller()->getRenderLoops() ) );
    this->set_debugInfo_topRight( fmt::format( fmt::runtime( "{:.1F} llps" ), this->sdlWindow_->get_performance_controller()->getLogicLoops() ) );
    this->set_debugInfo_bottomLeft( fmt::format( fmt::runtime( "{:.1F} ilps" ), this->sdlWindow_->get_performance_controller()->getInputLoops() ) );
    this->set_debugInfo_bottomRight( fmt::format( fmt::runtime( "{:.1F} nlps" ), this->sdlWindow_->get_performance_controller()->getNetworkLoops() ) );

    this->countDown1s_ = 50;  // fixed_update is called at 50hz
  }
}

void SFG::Content::DebugInfo::network_update() {
  SFG::Engine::Script::network_update();
}

void SFG::Content::DebugInfo::end() {
  SFG::Engine::Script::end();
}

void SFG::Content::DebugInfo::set_debugInfo_topLeft( std::string const& debugInfo ) {
  this->debugInfoTopLeft_.message_ = debugInfo;
  this->debugInfoTopLeft_.drawNew_ = true;
}

void SFG::Content::DebugInfo::set_debugInfo_topRight( std::string const& debugInfo ) {
  this->debugInfoTopRight_.message_ = debugInfo;
  this->debugInfoTopRight_.drawNew_ = true;
}

void SFG::Content::DebugInfo::set_debugInfo_bottomLeft( std::string const& debugInfo ) {
  this->debugInfoBottomLeft_.message_ = debugInfo;
  this->debugInfoBottomLeft_.drawNew_ = true;
}

void SFG::Content::DebugInfo::set_debugInfo_bottomRight( std::string const& debugInfo ) {
  this->debugInfoBottomRight_.message_ = debugInfo;
  this->debugInfoBottomRight_.drawNew_ = true;
}

void SFG::Content::DebugInfo::renderDebugInfo( DebugInfoStruct& debugInfo, SDL_Renderer const* renderer ) {
  SDL_Surface* txtsfc = TTF_RenderUTF8_Shaded_Wrapped( this->sdlFont_,
                                                       debugInfo.message_.c_str(),
                                                       SDL_Color( 255, 255, 255, 255 ),
                                                       SDL_Color( 0, 0, 0, 128 ),
                                                       this->sdlWindow_->get_width() );
  if( txtsfc == nullptr ) {
    this->logger_->error( fmt::runtime( "renderDebugInfo - Error when TTF_RenderUTF8_Shaded_Wrapped: {:s}" ), TTF_GetError() );
  } else {
    if( debugInfo.texture_ ) {
      SDL_DestroyTexture( debugInfo.texture_ );
    }
    debugInfo.texture_ = SDL_CreateTextureFromSurface( const_cast< SDL_Renderer* >( renderer ), txtsfc );
    if( !debugInfo.texture_ ) {
      this->logger_->error( fmt::runtime( "renderDebugInfo - Error when SDL_CreateTextureFromSurface: {:s}" ), SDL_GetError() );
    }
    debugInfo.textureRect_.w = txtsfc->w;
    debugInfo.textureRect_.h = txtsfc->h;
    SDL_FreeSurface( txtsfc );
    debugInfo.drawNew_ = false;
  }
}
