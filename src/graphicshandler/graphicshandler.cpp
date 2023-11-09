#include "graphicshandler/graphicshandler.h"
#include "performance/performance.h"

namespace SFG {
GraphicsHandler::GraphicsHandler( std::shared_ptr< SDL_Window > window )
    : logger( spdlog::get( "GraphicsHandler" ) ), drawCallbacks(), quitFlag( nullptr ), rendererIndex( 0 ), window( window ), windowRenderer( nullptr ) {
  logger->trace( fmt::runtime( "GraphicsHandler(std::shared_ptr<SDL_Window> window = {:p})" ), static_cast< void* >( window.get() ) );
  logger->trace( fmt::runtime( "GraphicsHandler()~" ) );
}

GraphicsHandler::~GraphicsHandler() {
  logger->trace( fmt::runtime( "~GraphicsHandler()" ) );
  logger->trace( fmt::runtime( "~GraphicsHandler()~" ) );
}

void GraphicsHandler::deleteRenderer( SDL_Renderer* ptr ) {
  logger->trace( fmt::runtime( "Deleting renderer {:p}" ), static_cast< void* >( ptr ) );
  SDL_DestroyRenderer( ptr );
}

void GraphicsHandler::Draw( GraphicsHandler* self ) {
  self->logger->trace( fmt::runtime( "Draw(GraphicsHandler* self = {:p})" ), static_cast< void* >( self ) );

  std::function< void( SDL_Renderer* ) > rendererDeleteFunction = [self]( SDL_Renderer* ptr ) { self->deleteRenderer( ptr ); };

  self->windowRenderer = std::shared_ptr< SDL_Renderer >( SDL_CreateRenderer( self->window.get(), self->rendererIndex, 0 ), rendererDeleteFunction );
  if( self->windowRenderer.get() == nullptr ) {
    self->logger->error( fmt::runtime( "Draw - Error when SDL_CreateRenderer: {:s}" ), SDL_GetError() );
  }
  if( SDL_SetRenderDrawColor( self->windowRenderer.get(), 0, 0, 0, 255 ) != 0 ) {
    self->logger->error( fmt::runtime( "Draw - Error when SDL_SetRenderDrawColor: {:s}" ), SDL_GetError() );
  }
  while( !( *self->quitFlag ) ) {
    if( SDL_RenderClear( self->windowRenderer.get() ) != 0 ) {
      self->logger->error( fmt::runtime( "Draw - Error when SDL_RenderClear: {:s}" ), SDL_GetError() );
    }

    for( auto callback : self->drawCallbacks )
      callback( self->windowRenderer );

    SDL_RenderPresent( self->windowRenderer.get() );
    Performance::AddGraphicsLoop();
  }
  self->logger->trace( fmt::runtime( "Draw()~" ) );
}

void GraphicsHandler::SetQuitFlag( std::shared_ptr< bool > newQuitFlag ) {
  logger->trace( fmt::runtime( "SetQuitFlag(std::shared_ptr<bool> quitFlag = {:p})" ), static_cast< void* >( newQuitFlag.get() ) );
  if( newQuitFlag )
    this->quitFlag = newQuitFlag;
  logger->trace( fmt::runtime( "SetQuitFlag()~" ) );
}

void GraphicsHandler::SetRendererIndex( int index ) {
  logger->trace( fmt::runtime( "SetRendererIndex(int index = {:d})" ), index );
  if( index >= 0 )
    this->rendererIndex = index;
  logger->trace( fmt::runtime( "SetRendererIndex()~" ) );
}

void GraphicsHandler::StartDraw() {
  logger->trace( fmt::runtime( "StartDraw()" ) );
  graphicsThread = std::thread( Draw, this );
  logger->trace( fmt::runtime( "StartDraw()~" ) );
}

void GraphicsHandler::StopDraw() {
  logger->trace( fmt::runtime( "StopDraw()" ) );
  graphicsThread.join();
  logger->trace( fmt::runtime( "StopDraw()~" ) );
}

void GraphicsHandler::RegisterDrawEvent( DrawCallback callback ) {
  logger->trace( fmt::runtime( "RegisterDrawEvent(DrawCallback callback)" ) );
  if( callback )
    drawCallbacks.push_back( callback );
  logger->trace( fmt::runtime( "RegisterDrawEvent()~" ) );
}
}  // namespace SFG
