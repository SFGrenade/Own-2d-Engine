#include <SFG/Own2dEngine/Logger/_include.h>
#include <gtest/gtest.h>

TEST( Logger, ScopedLogger ) {
  { SFG::Own2dEngine::Logger::ScopedLogger( SFG::Own2dEngine::Logger::LoggerFactory::get_logger( "ScopedLogger" ), "Entry", "Exit" ); }
  EXPECT_TRUE( true );
}
