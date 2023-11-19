#include "graphics/graphicshandler.h"

#include "_globals/misc.h"
#include "confighandler/confighandler.h"
#include "graphics/fonthandler.h"

namespace SFG {
void GraphicsHandler::deleteRenderer( SDL_Renderer* ptr ) {
  this->logger_->trace( fmt::runtime( "deleteRenderer( ptr = {:p} )" ), static_cast< void* >( ptr ) );

  SDL_DestroyRenderer( ptr );

  this->logger_->trace( fmt::runtime( "deleteRenderer()~" ) );
}

GraphicsHandler::GraphicsHandler( std::shared_ptr< SDL_Window > window )
    : logger_( spdlog::get( "GraphicsHandler" ) ),
      fontHandler_( new FontHandler() ),
      drawCallbacks_(),
      rendererIndex_( 0 ),
      window_( window ),
      windowRenderer_( nullptr ),
      debugInfo_texture_( nullptr ),
      debugInfo_textureRect_(),
      debugInfo_foregroundColor_(),
      debugInfo_backgroundColor_() {
  this->logger_->trace( fmt::runtime( "GraphicsHandler( window = {:p} )" ), static_cast< void* >( window.get() ) );

  std::vector< std::string > fgCol = str_split( SFG::ConfigHandler::get_Rendering_PerformanceStringTextColor(), " " );
  std::vector< std::string > bgCol = str_split( SFG::ConfigHandler::get_Rendering_PerformanceStringBackgroundColor(), " " );
  debugInfo_foregroundColor_.r = static_cast< uint8_t >( std::stoi( fgCol[0] ) );
  debugInfo_foregroundColor_.g = static_cast< uint8_t >( std::stoi( fgCol[1] ) );
  debugInfo_foregroundColor_.b = static_cast< uint8_t >( std::stoi( fgCol[2] ) );
  debugInfo_foregroundColor_.a = static_cast< uint8_t >( std::stoi( fgCol[3] ) );
  debugInfo_backgroundColor_.r = static_cast< uint8_t >( std::stoi( bgCol[0] ) );
  debugInfo_backgroundColor_.g = static_cast< uint8_t >( std::stoi( bgCol[1] ) );
  debugInfo_backgroundColor_.b = static_cast< uint8_t >( std::stoi( bgCol[2] ) );
  debugInfo_backgroundColor_.a = static_cast< uint8_t >( std::stoi( bgCol[3] ) );

  this->logger_->trace( fmt::runtime( "GraphicsHandler()~" ) );
}

GraphicsHandler::~GraphicsHandler() {
  this->logger_->trace( fmt::runtime( "~GraphicsHandler()" ) );

  if( debugInfo_texture_ )
    SDL_DestroyTexture( debugInfo_texture_ );

  this->logger_->trace( fmt::runtime( "~GraphicsHandler()~" ) );
}

void GraphicsHandler::SetRendererIndex( int index ) {
  this->logger_->trace( fmt::runtime( "SetRendererIndex( index = {:d} )" ), index );

  if( index >= 0 ) {
    this->rendererIndex_ = index;
  }

  this->logger_->trace( fmt::runtime( "SetRendererIndex()~" ) );
}

void GraphicsHandler::StartDraw() {
  this->logger_->trace( fmt::runtime( "StartDraw()" ) );

  this->windowRenderer_ = std::shared_ptr< SDL_Renderer >( SDL_CreateRenderer( this->window_.get(), this->rendererIndex_, 0 ),
                                                           [this]( SDL_Renderer* ptr ) { this->deleteRenderer( ptr ); } );
  if( this->windowRenderer_.get() == nullptr ) {
    this->logger_->error( fmt::runtime( "Draw - Error when SDL_CreateRenderer: {:s}" ), SDL_GetError() );
  }
  if( SDL_SetRenderDrawColor( this->windowRenderer_.get(), 0, 0, 0, 255 ) != 0 ) {
    this->logger_->error( fmt::runtime( "Draw - Error when SDL_SetRenderDrawColor: {:s}" ), SDL_GetError() );
  }

  this->logger_->trace( fmt::runtime( "StartDraw()~" ) );
}

void GraphicsHandler::Draw() {
  // this->logger_->trace( fmt::runtime( "Draw()" ) );

  if( SDL_RenderClear( this->windowRenderer_.get() ) != 0 ) {
    this->logger_->error( fmt::runtime( "Draw - Error when SDL_RenderClear: {:s}" ), SDL_GetError() );
  }
  for( auto callback : this->drawCallbacks_ ) {
    callback( this->windowRenderer_ );
  }
  if( this->debugInfo_texture_ ) {
    if( SDL_RenderCopy( this->windowRenderer_.get(), this->debugInfo_texture_, NULL, &this->debugInfo_textureRect_ ) != 0 ) {
      spdlog::error( fmt::runtime( "Draw - Error when SDL_RenderCopy: {:s}" ), SDL_GetError() );
    }
  }
  SDL_RenderPresent( this->windowRenderer_.get() );

  // this->logger_->trace( fmt::runtime( "Draw()~" ) );
}

void GraphicsHandler::RegisterDrawEvent( DrawCallback callback ) {
  this->logger_->trace( fmt::runtime( "RegisterDrawEvent( callback )" ) );

  if( callback ) {
    this->drawCallbacks_.push_back( callback );
  }

  this->logger_->trace( fmt::runtime( "RegisterDrawEvent()~" ) );
}

void GraphicsHandler::SetDebugString( std::string debugString ) {
  // this->logger_->trace( fmt::runtime( "SetDebugString( debugString = \"{:s}\" )" ), debugString );

  SDL_Surface* textSurface = TTF_RenderUTF8_Shaded_Wrapped( this->fontHandler_->GetFont( SFG::FontType::Console ).get(),
                                                            debugString.c_str(),
                                                            this->debugInfo_foregroundColor_,
                                                            this->debugInfo_backgroundColor_,
                                                            0 );
  if( this->debugInfo_texture_ )
    SDL_DestroyTexture( this->debugInfo_texture_ );
  this->debugInfo_texture_ = SDL_CreateTextureFromSurface( this->windowRenderer_.get(), textSurface );
  this->debugInfo_textureRect_.w = textSurface->w;
  this->debugInfo_textureRect_.h = textSurface->h;
  SDL_FreeSurface( textSurface );

  // this->logger_->trace( fmt::runtime( "SetDebugString()~" ) );
}
}  // namespace SFG
