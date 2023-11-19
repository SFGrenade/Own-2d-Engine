#include "graphics/fonthandler.h"

#include "confighandler/confighandler.h"

namespace SFG {
void FontHandler::deleteFont( TTF_Font* ptr ) {
  this->logger_->trace( fmt::runtime( "deleteFont( ptr = {:p} )" ), static_cast< void* >( ptr ) );

  TTF_CloseFont( ptr );

  this->logger_->trace( fmt::runtime( "deleteFont()~" ) );
}

FontHandler::FontHandler() : logger_( spdlog::get( "FontHandler" ) ), fonts_() {
  this->logger_->trace( fmt::runtime( "FontHandler()" ) );

  if( TTF_Init() != 0 ) {
    this->logger_->error( fmt::runtime( "FontHandler - Error when TTF_Init: {:s}" ), TTF_GetError() );
  }

  this->fonts_[FontType::Console]
      = std::shared_ptr< TTF_Font >( TTF_OpenFont( "./Resources/Fonts/NotoSansMono-Regular.ttf", ConfigHandler::get_Font_PixelSize() ),
                                     [this]( TTF_Font* ptr ) { this->deleteFont( ptr ); } );
  if( this->fonts_[FontType::Console].get() == nullptr ) {
    this->logger_->error( fmt::runtime( "FontHandler - Error when creating font `Console`: {:s}" ), TTF_GetError() );
  }
  this->fonts_[FontType::Ui] = std::shared_ptr< TTF_Font >( TTF_OpenFont( "./Resources/Fonts/NotoSerif-Regular.ttf", ConfigHandler::get_Font_PixelSize() ),
                                                            [this]( TTF_Font* ptr ) { this->deleteFont( ptr ); } );
  if( this->fonts_[FontType::Ui].get() == nullptr ) {
    this->logger_->error( fmt::runtime( "FontHandler - Error when creating font `Ui`: {:s}" ), TTF_GetError() );
  }

  this->logger_->trace( fmt::runtime( "FontHandler()~" ) );
}

FontHandler::~FontHandler() {
  this->logger_->trace( fmt::runtime( "~FontHandler()" ) );

  this->fonts_.clear();

  TTF_Quit();

  this->logger_->trace( fmt::runtime( "~FontHandler()~" ) );
}

std::shared_ptr< TTF_Font > FontHandler::GetFont( FontType type ) {
  // this->logger_->trace( fmt::runtime( "GetFont( type = {:d} )" ), static_cast< int >( type ) );

  // this->logger_->trace( fmt::runtime( "GetFont()~" ) );
  return this->fonts_[type];
}
}  // namespace SFG
