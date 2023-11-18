#include "graphics/graphicshandler.h"

#include "_globals/misc.h"
#include "confighandler/confighandler.h"
#include "graphics/fonthandler.h"

namespace SFG {
void GraphicsHandler::deleteRenderer( SDL_Renderer* ptr ) {
  logger->trace( fmt::runtime( "deleteRenderer(SDL_Renderer* ptr = {:p})" ), static_cast< void* >( ptr ) );
  SDL_DestroyRenderer( ptr );
  logger->trace( fmt::runtime( "deleteRenderer()~" ) );
}

GraphicsHandler::GraphicsHandler( std::shared_ptr< SDL_Window > window )
    : logger( spdlog::get( "GraphicsHandler" ) ),
      drawCallbacks(),
      rendererIndex( 0 ),
      window( window ),
      windowRenderer( nullptr ),
      debugInfo_texture_( nullptr ),
      debugInfo_textureRect_(),
      debugInfo_foregroundColor_(),
      debugInfo_backgroundColor_() {
  logger->trace( fmt::runtime( "GraphicsHandler(std::shared_ptr<SDL_Window> window = {:p})" ), static_cast< void* >( window.get() ) );

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

  logger->trace( fmt::runtime( "GraphicsHandler()~" ) );
}

GraphicsHandler::~GraphicsHandler() {
  logger->trace( fmt::runtime( "~GraphicsHandler()" ) );

  if( debugInfo_texture_ )
    SDL_DestroyTexture( debugInfo_texture_ );

  logger->trace( fmt::runtime( "~GraphicsHandler()~" ) );
}

void GraphicsHandler::SetRendererIndex( int index ) {
  logger->trace( fmt::runtime( "SetRendererIndex(int index = {:d})" ), index );

  if( index >= 0 ) {
    this->rendererIndex = index;
  }

  logger->trace( fmt::runtime( "SetRendererIndex()~" ) );
}

void GraphicsHandler::StartDraw() {
  logger->trace( fmt::runtime( "StartDraw()" ) );

  this->windowRenderer = std::shared_ptr< SDL_Renderer >( SDL_CreateRenderer( this->window.get(), this->rendererIndex, 0 ),
                                                          [this]( SDL_Renderer* ptr ) { this->deleteRenderer( ptr ); } );
  if( this->windowRenderer.get() == nullptr ) {
    this->logger->error( fmt::runtime( "Draw - Error when SDL_CreateRenderer: {:s}" ), SDL_GetError() );
  }
  if( SDL_SetRenderDrawColor( this->windowRenderer.get(), 0, 0, 0, 255 ) != 0 ) {
    this->logger->error( fmt::runtime( "Draw - Error when SDL_SetRenderDrawColor: {:s}" ), SDL_GetError() );
  }

  logger->trace( fmt::runtime( "StartDraw()~" ) );
}

void GraphicsHandler::Draw() {
  // logger->trace( fmt::runtime( "Draw()" ) );

  if( SDL_RenderClear( this->windowRenderer.get() ) != 0 ) {
    this->logger->error( fmt::runtime( "Draw - Error when SDL_RenderClear: {:s}" ), SDL_GetError() );
  }
  for( auto callback : this->drawCallbacks ) {
    callback( this->windowRenderer );
  }
  SDL_RenderPresent( this->windowRenderer.get() );

  // logger->trace( fmt::runtime( "Draw()~" ) );
}

void GraphicsHandler::RegisterDrawEvent( DrawCallback callback ) {
  logger->trace( fmt::runtime( "RegisterDrawEvent(DrawCallback callback)" ) );

  if( callback ) {
    drawCallbacks.push_back( callback );
  }

  logger->trace( fmt::runtime( "RegisterDrawEvent()~" ) );
}

void GraphicsHandler::SetDebugString( std::string debugString ) {
  logger->trace( fmt::runtime( "SetDebugString(std::string debugString = \"{:s}\")" ), debugString );

  SDL_Surface* textSurface = TTF_RenderUTF8_Shaded_Wrapped( SFG::FontHandler::GetFont( SFG::FontType::Console ).get(),
                                                            debugString.c_str(),
                                                            debugInfo_foregroundColor_,
                                                            debugInfo_backgroundColor_,
                                                            0 );
  if( debugInfo_texture_ )
    SDL_DestroyTexture( debugInfo_texture_ );
  debugInfo_texture_ = SDL_CreateTextureFromSurface( windowRenderer.get(), textSurface );
  debugInfo_textureRect_.w = textSurface->w;
  debugInfo_textureRect_.h = textSurface->h;
  SDL_FreeSurface( textSurface );

  logger->trace( fmt::runtime( "SetDebugString()~" ) );
}
}  // namespace SFG
