#include "SFG/Own2dEngine/Engine/rendererManager.h"

#include "SFG/Own2dEngine/Engine/performance.h"

namespace SFG {
namespace Own2dEngine {
namespace Engine {

SFG::Own2dEngine::Logger::spdlogger RendererManager::logger_ = nullptr;
std::map< SDL_Window*, SDL_Renderer* > RendererManager::renderers_;

void RendererManager::init() {
  RendererManager::logger_ = SFG::Own2dEngine::Logger::LoggerFactory::get_logger( "RendererManager" );
}

std::string sdlEnum2String( SDL_RendererFlags flags ) {
  std::string ret = "";

  if( flags & SDL_RendererFlags::SDL_RENDERER_SOFTWARE )
    ret += "SDL_RENDERER_SOFTWARE | ";
  if( flags & SDL_RendererFlags::SDL_RENDERER_ACCELERATED )
    ret += "SDL_RENDERER_ACCELERATED | ";
  if( flags & SDL_RendererFlags::SDL_RENDERER_PRESENTVSYNC )
    ret += "SDL_RENDERER_PRESENTVSYNC | ";
  if( flags & SDL_RendererFlags::SDL_RENDERER_TARGETTEXTURE )
    ret += "SDL_RENDERER_TARGETTEXTURE | ";

  if( ret == "" ) {
    return "__UNKNOWN__";
  }
  return ret.substr( 0, ret.size() - 3 );
}

std::string sdlEnum2String( SDL_PixelFormatEnum flags ) {
  uint8_t pixelType = SDL_PIXELTYPE( flags );
  uint8_t pixelOrder = SDL_PIXELORDER( flags );
  uint8_t pixelLayout = SDL_PIXELLAYOUT( flags );
  uint8_t pixelBitDepth = SDL_BITSPERPIXEL( flags );
  uint8_t pixelByteDepth = SDL_BYTESPERPIXEL( flags );
  bool pixelIsIndexed = SDL_ISPIXELFORMAT_INDEXED( flags );
  bool pixelHasAlpha = SDL_ISPIXELFORMAT_ALPHA( flags );
  bool pixelIsFourCC = SDL_ISPIXELFORMAT_FOURCC( flags );

  std::string typeString = "__UNKNOWN__";
  if( pixelType == SDL_PIXELTYPE_UNKNOWN )
    typeString = "UNKNOWN";
  else if( pixelType == SDL_PIXELTYPE_INDEX1 )
    typeString = "INDEX1";
  else if( pixelType == SDL_PIXELTYPE_INDEX4 )
    typeString = "INDEX4";
  else if( pixelType == SDL_PIXELTYPE_INDEX8 )
    typeString = "INDEX8";
  else if( pixelType == SDL_PIXELTYPE_PACKED8 )
    typeString = "PACKED8";
  else if( pixelType == SDL_PIXELTYPE_PACKED16 )
    typeString = "PACKED16";
  else if( pixelType == SDL_PIXELTYPE_PACKED32 )
    typeString = "PACKED32";
  else if( pixelType == SDL_PIXELTYPE_ARRAYU8 )
    typeString = "ARRAYU8";
  else if( pixelType == SDL_PIXELTYPE_ARRAYU16 )
    typeString = "ARRAYU16";
  else if( pixelType == SDL_PIXELTYPE_ARRAYU32 )
    typeString = "ARRAYU32";
  else if( pixelType == SDL_PIXELTYPE_ARRAYF16 )
    typeString = "ARRAYF16";
  else if( pixelType == SDL_PIXELTYPE_ARRAYF32 )
    typeString = "ARRAYF32";
  else if( pixelType == SDL_PIXELTYPE_INDEX2 )
    typeString = "INDEX2";

  std::string orderString = "__UNKNOWN__";
  if( pixelType == SDL_PIXELTYPE_UNKNOWN || pixelType == SDL_PIXELTYPE_INDEX1 || pixelType == SDL_PIXELTYPE_INDEX4 || pixelType == SDL_PIXELTYPE_INDEX8
      || pixelType == SDL_PIXELTYPE_INDEX2 ) {
    // bitmap ordering
    if( pixelOrder == SDL_BITMAPORDER_NONE )
      orderString = "NONE";
    else if( pixelOrder == SDL_BITMAPORDER_4321 )
      orderString = "4321";
    else if( pixelOrder == SDL_BITMAPORDER_1234 )
      orderString = "1234";
  } else if( pixelType == SDL_PIXELTYPE_PACKED8 || pixelType == SDL_PIXELTYPE_PACKED16 || pixelType == SDL_PIXELTYPE_PACKED32 ) {
    // packed ordering
    if( pixelOrder == SDL_PACKEDORDER_NONE )
      orderString = "NONE";
    else if( pixelOrder == SDL_PACKEDORDER_XRGB )
      orderString = "XRGB";
    else if( pixelOrder == SDL_PACKEDORDER_RGBX )
      orderString = "RGBX";
    else if( pixelOrder == SDL_PACKEDORDER_ARGB )
      orderString = "ARGB";
    else if( pixelOrder == SDL_PACKEDORDER_RGBA )
      orderString = "RGBA";
    else if( pixelOrder == SDL_PACKEDORDER_XBGR )
      orderString = "XBGR";
    else if( pixelOrder == SDL_PACKEDORDER_BGRX )
      orderString = "BGRX";
    else if( pixelOrder == SDL_PACKEDORDER_ABGR )
      orderString = "ABGR";
    else if( pixelOrder == SDL_PACKEDORDER_BGRA )
      orderString = "BGRA";
  } else if( pixelType == SDL_PIXELTYPE_ARRAYU8 || pixelType == SDL_PIXELTYPE_ARRAYU16 || pixelType == SDL_PIXELTYPE_ARRAYU32
             || pixelType == SDL_PIXELTYPE_ARRAYF16 || pixelType == SDL_PIXELTYPE_ARRAYF32 ) {
    // array ordering
    if( pixelOrder == SDL_ARRAYORDER_NONE )
      orderString = "NONE";
    else if( pixelOrder == SDL_ARRAYORDER_RGB )
      orderString = "RGB";
    else if( pixelOrder == SDL_ARRAYORDER_RGBA )
      orderString = "RGBA";
    else if( pixelOrder == SDL_ARRAYORDER_ARGB )
      orderString = "ARGB";
    else if( pixelOrder == SDL_ARRAYORDER_BGR )
      orderString = "BGR";
    else if( pixelOrder == SDL_ARRAYORDER_BGRA )
      orderString = "BGRA";
    else if( pixelOrder == SDL_ARRAYORDER_ABGR )
      orderString = "ABGR";
  }

  std::string layoutString = "__UNKNOWN__";
  if( pixelLayout == SDL_PACKEDLAYOUT_NONE )
    layoutString = "NONE";
  else if( pixelLayout == SDL_PACKEDLAYOUT_332 )
    layoutString = "332";
  else if( pixelLayout == SDL_PACKEDLAYOUT_4444 )
    layoutString = "4444";
  else if( pixelLayout == SDL_PACKEDLAYOUT_1555 )
    layoutString = "1555";
  else if( pixelLayout == SDL_PACKEDLAYOUT_5551 )
    layoutString = "5551";
  else if( pixelLayout == SDL_PACKEDLAYOUT_565 )
    layoutString = "565";
  else if( pixelLayout == SDL_PACKEDLAYOUT_8888 )
    layoutString = "8888";
  else if( pixelLayout == SDL_PACKEDLAYOUT_2101010 )
    layoutString = "2101010";
  else if( pixelLayout == SDL_PACKEDLAYOUT_1010102 )
    layoutString = "1010102";

  return fmt::format( "Type: {:s}, Order: {:s}, Layout: {:s}, BitDepth: {:d}, ByteDepth: {:d}, IsIndexed: {}, HasAlpha: {}, IsFourCC: {}",
                      typeString,
                      orderString,
                      layoutString,
                      pixelBitDepth,
                      pixelByteDepth,
                      pixelIsIndexed,
                      pixelHasAlpha,
                      pixelIsFourCC );
}

void RendererManager::GetRendererInfos( SDL_Renderer* renderer ) {
  RendererManager::logger_->trace( "GetRendererInfos( renderer: {:p} )", static_cast< void* >( renderer ) );

  SDL_RendererInfo rendererInfo;

  if( renderer == nullptr ) {
    int numRenderDrivers = SDL_GetNumRenderDrivers();
    if( numRenderDrivers < 0 ) {
      RendererManager::logger_->error( "CreateRenderer - error calling SDL_GetNumRenderDrivers: {:s}", SDL_GetError() );
    } else {
      for( int i = 0; i < numRenderDrivers; i++ ) {
        if( SDL_GetRenderDriverInfo( i, &rendererInfo ) != 0 ) {
          RendererManager::logger_->error( "GetRendererInfos - error calling SDL_GetRenderDriverInfo: {:s}", SDL_GetError() );
          continue;
        }
        RendererManager::logger_->info( "GetRendererInfos - index {:d}:", i );
        RendererManager::logger_->info( "GetRendererInfos -   - name: {:?}", rendererInfo.name );
        RendererManager::logger_->info( "GetRendererInfos -   - flags: {:s}", sdlEnum2String( static_cast< SDL_RendererFlags >( rendererInfo.flags ) ) );
        RendererManager::logger_->info( "GetRendererInfos -   - num_texture_formats: {:d}", rendererInfo.num_texture_formats );
        for( uint32_t o = 0; o < rendererInfo.num_texture_formats; o++ ) {
          RendererManager::logger_->info( "GetRendererInfos -       - {:s}",
                                          sdlEnum2String( static_cast< SDL_PixelFormatEnum >( rendererInfo.texture_formats[o] ) ) );
        }
        RendererManager::logger_->info( "GetRendererInfos -   - max_texture_width: {:d}", rendererInfo.max_texture_width );
        RendererManager::logger_->info( "GetRendererInfos -   - max_texture_height: {:d}", rendererInfo.max_texture_height );
      }
    }
  } else {
    if( SDL_GetRendererInfo( renderer, &rendererInfo ) != 0 ) {
      RendererManager::logger_->error( "GetRendererInfos - error calling SDL_GetRenderDriverInfo: {:s}", SDL_GetError() );
      return;
    }
    RendererManager::logger_->info( "GetRendererInfos - name: {:?}", rendererInfo.name );
    RendererManager::logger_->info( "GetRendererInfos - flags: {:s}", sdlEnum2String( static_cast< SDL_RendererFlags >( rendererInfo.flags ) ) );
    RendererManager::logger_->info( "GetRendererInfos - num_texture_formats: {:d}", rendererInfo.num_texture_formats );
    for( uint32_t o = 0; o < rendererInfo.num_texture_formats; o++ ) {
      RendererManager::logger_->info( "GetRendererInfos -   - {:s}", sdlEnum2String( static_cast< SDL_PixelFormatEnum >( rendererInfo.texture_formats[o] ) ) );
    }
    RendererManager::logger_->info( "GetRendererInfos - max_texture_width: {:d}", rendererInfo.max_texture_width );
    RendererManager::logger_->info( "GetRendererInfos - max_texture_height: {:d}", rendererInfo.max_texture_height );
  }
}

SDL_Renderer* RendererManager::CreateRenderer( SDL_Window* window, SDL_RendererFlags flags, std::string const& wantedRenderer ) {
  RendererManager::logger_->trace( "CreateRenderer( window: {:p}, flags: {:s}, renderer: {:?} )",
                                   static_cast< void* >( window ),
                                   sdlEnum2String( flags ),
                                   wantedRenderer );

  SDL_RendererInfo rendererInfo;
  SDL_Renderer* renderer = nullptr;
  bool foundRenderer = false;

  if( wantedRenderer == "default" ) {
    foundRenderer = true;
    renderer = SDL_CreateRenderer( window, -1, static_cast< int >( flags ) );
  } else {
    int numRenderDrivers = SDL_GetNumRenderDrivers();
    if( numRenderDrivers < 0 ) {
      RendererManager::logger_->error( "CreateRenderer - error calling SDL_GetNumRenderDrivers: {:s}", SDL_GetError() );
    } else {
      for( int i = 0; i < numRenderDrivers; i++ ) {
        if( SDL_GetRenderDriverInfo( i, &rendererInfo ) != 0 ) {
          RendererManager::logger_->error( "CreateRenderer - error calling SDL_GetRenderDriverInfo: {:s}", SDL_GetError() );
        }
        std::string rendererName( rendererInfo.name );
        if( wantedRenderer == rendererName ) {
          // we want to use this renderer
          foundRenderer = true;
          renderer = SDL_CreateRenderer( window, i, static_cast< int >( flags ) );
          break;
        }
      }
    }
  }
  if( ( renderer == nullptr ) && !foundRenderer ) {
    RendererManager::logger_->error( "CreateRenderer - no renderer {:?} found", wantedRenderer );
    return nullptr;
  }
  if( ( renderer == nullptr ) && foundRenderer ) {
    RendererManager::logger_->error( "CreateRenderer - error calling SDL_CreateRenderer: {:s}", SDL_GetError() );
    return nullptr;
  }
  RendererManager::renderers_.emplace( window, renderer );

  return renderer;
}

void RendererManager::DoRender( SDL_Renderer* renderer, std::function< void( SDL_Renderer* ) > callback ) {
  std::string rendererCategory = fmt::format( "Rendering {:p}", static_cast< void* >( renderer ) );

  Performance::startTiming( rendererCategory );
  SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xff );
  SDL_RenderClear( renderer );

  if( callback != nullptr ) {
    callback( renderer );
  }

  SDL_RenderPresent( renderer );
  Performance::endTiming( rendererCategory );
}

void RendererManager::DestroyRendererForWindow( SDL_Window* window ) {
  RendererManager::logger_->trace( "DestroyRendererForWindow( window: {:p} )", static_cast< void* >( window ) );

  if( !RendererManager::renderers_.contains( window ) ) {
    RendererManager::logger_->warn( "DestroyRendererForWindow - window {:p} not found in map, ignoring", static_cast< void* >( window ) );
    return;
  }
  SDL_DestroyRenderer( RendererManager::renderers_.at( window ) );
  RendererManager::renderers_.erase( window );
}

void RendererManager::Shutdown() {
  RendererManager::logger_->trace( "Shutdown()" );

  for( std::pair< SDL_Window*, SDL_Renderer* > const& pair : RendererManager::renderers_ ) {
    SDL_DestroyRenderer( pair.second );
  }
  RendererManager::renderers_.clear();
}

}  // namespace Engine
}  // namespace Own2dEngine
}  // namespace SFG
