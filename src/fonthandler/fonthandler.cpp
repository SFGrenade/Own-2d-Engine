#include "confighandler/confighandler.h"
#include "fonthandler/fonthandler.h"

namespace SFG {
spdlogger FontHandler::logger = nullptr;
std::map< FontType, std::shared_ptr< TTF_Font > > FontHandler::fonts = std::map< FontType, std::shared_ptr< TTF_Font > >();

void FontHandler::deleteFont( TTF_Font* ptr ) {
  FontHandler::logger->trace( fmt::runtime( "Deleting font '{:s}'/'{:s}'" ), TTF_FontFaceFamilyName( ptr ), TTF_FontFaceStyleName( ptr ) );
  TTF_CloseFont( ptr );
}

void FontHandler::Initialize() {
  FontHandler::logger = spdlog::get( "FontHandler" );
  FontHandler::logger->trace( fmt::runtime( "Initialize()" ) );
  if( TTF_Init() != 0 ) {
    FontHandler::logger->error( fmt::runtime( "Initialize - Error when TTF_Init: {:s}" ), TTF_GetError() );
  }

  FontHandler::fonts[FontType::Console] = std::shared_ptr< TTF_Font >( TTF_OpenFont( "./Resources/Fonts/NotoSansMono-Regular.ttf", ConfigHandler::get_Font_PixelSize() ), FontHandler::deleteFont );
  if( FontHandler::fonts[FontType::Console].get() == nullptr ) {
    FontHandler::logger->error( fmt::runtime( "Initialize - Error when creating font `Console`: {:s}" ), TTF_GetError() );
  }
  FontHandler::fonts[FontType::Ui] = std::shared_ptr< TTF_Font >( TTF_OpenFont( "./Resources/Fonts/NotoSerif-Regular.ttf", ConfigHandler::get_Font_PixelSize() ), FontHandler::deleteFont );
  if( FontHandler::fonts[FontType::Ui].get() == nullptr ) {
    FontHandler::logger->error( fmt::runtime( "Initialize - Error when creating font `Ui`: {:s}" ), TTF_GetError() );
  }
  FontHandler::logger->trace( fmt::runtime( "Initialize()~" ) );
}

void FontHandler::Destroy() {
  FontHandler::logger->trace( fmt::runtime( "Destroy()" ) );
  FontHandler::fonts.clear();

  TTF_Quit();
  FontHandler::logger->trace( fmt::runtime( "Destroy()~" ) );
}

std::shared_ptr< TTF_Font > FontHandler::GetFont( FontType type ) {
  FontHandler::logger->trace( fmt::runtime( "GetFont(FontType type = {:d})" ), static_cast< int >( type ) );
  FontHandler::logger->trace( fmt::runtime( "GetFont()~" ) );
  return FontHandler::fonts[type];
}
}  // namespace SFG
