#include "fonthandler/fonthandler.h"

namespace SFG {
spdlogger FontHandler::logger = nullptr;
std::map< FontType, std::shared_ptr< TTF_Font > > FontHandler::fonts = std::map< FontType, std::shared_ptr< TTF_Font > >();

void FontHandler::deleteFont( TTF_Font* ptr ) {
  FontHandler::logger->trace( "Deleting font '{:s}'/'{:s}'", TTF_FontFaceFamilyName( ptr ), TTF_FontFaceStyleName( ptr ) );
  TTF_CloseFont( ptr );
}

void FontHandler::Initialize() {
  FontHandler::logger = spdlog::get( "FontHandler" );
  FontHandler::logger->trace( "Initialize()" );
  TTF_Init();

  FontHandler::fonts[FontType::Console]
      = std::shared_ptr< TTF_Font >( TTF_OpenFont( "./Resources/Fonts/NotoSansMono-Regular.ttf", 18 ), FontHandler::deleteFont );
  FontHandler::fonts[FontType::Ui] = std::shared_ptr< TTF_Font >( TTF_OpenFont( "./Resources/Fonts/NotoSerif-Regular.ttf", 18 ), FontHandler::deleteFont );
  FontHandler::logger->trace( "Initialize()~" );
}

void FontHandler::Destroy() {
  FontHandler::logger->trace( "Destroy()" );
  FontHandler::fonts.clear();

  TTF_Quit();
  FontHandler::logger->trace( "Destroy()~" );
}

std::shared_ptr< TTF_Font > FontHandler::GetFont( FontType type ) {
  FontHandler::logger->trace( "GetFont(FontType type = {:d})", type );
  FontHandler::logger->trace( "GetFont()~" );
  return FontHandler::fonts[type];
}
}  // namespace SFG
