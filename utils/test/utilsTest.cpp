#include <SFG/Own2dEngine/Logger/_include.h>
#include <SFG/Own2dEngine/Utils/_include.h>
#include <gtest/gtest.h>

TEST( Utils, Example ) {
  EXPECT_TRUE( true );
}

TEST( Utils, Align ) {
  struct tmp {
    UTILS_ALIGN( 64 ) uint16_t unused;
  } tmpStruct;

  EXPECT_EQ( 64, sizeof( tmpStruct ) );
}

TEST( Utils, Contains ) {
  std::vector< int > tmpVector{ 1, 2, 3, 4, 5 };
  EXPECT_TRUE( SFG::Own2dEngine::Utils::contains( tmpVector, 4 ) );
  EXPECT_FALSE( SFG::Own2dEngine::Utils::contains( tmpVector, 7 ) );
}

TEST( Utils, MoreChrono ) {
  std::chrono::high_resolution_clock::time_point current = std::chrono::high_resolution_clock::now();
  std::chrono::secondsLongDouble timePassed = SFG::Own2dEngine::Utils::getDurationSinceLast( current );
  EXPECT_TRUE( true );
}

TEST( Utils, NumOps ) {
  EXPECT_EQ( 5, std::max( 1, 5 ) );
  EXPECT_EQ( 1, std::min( 1, 5 ) );
  EXPECT_EQ( 5, std::clamp( 10, 1, 5 ) );
  EXPECT_EQ( 5.0, std::lerp( 0.0, 10.0, 0.5 ) );
}

TEST( Utils, StrSplit ) {
  std::string text = "Hello World";
  std::vector< std::string > splitted = SFG::Own2dEngine::Utils::strSplit( text, " " );
  EXPECT_EQ( 2, splitted.size() );
  EXPECT_EQ( std::string( "Hello" ), splitted.at( 0 ) );
  EXPECT_EQ( std::string( "World" ), splitted.at( 1 ) );
}
