#include <SFG/Own2dEngine/Logger/loggerFactory.h>
#include <SFG/Own2dEngine/Logger/scopedLogger.h>
#include <gtest/gtest.h>

TEST( Logger, ScopedLogger ) {
  { SFG::Own2dEngine::Logger::ScopedLogger( SFG::Own2dEngine::Logger::LoggerFactory::get_logger( "ScopedLogger" ), "Entry", "Exit" ); }
  EXPECT_TRUE( true );
}
