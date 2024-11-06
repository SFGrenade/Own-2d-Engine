#include <SFG/Own2dEngine/Configuration/configuration.h>
#include <SFG/Own2dEngine/Logger/loggerFactory.h>
#include <gtest/gtest.h>
#include <string>

TEST( Configuration, ReadingStringValue ) {
  SFG::Own2dEngine::Configuration::Configuration config( "config/test_conf.ini" );
  std::string myVal = config.get< std::string >( "Test", "Key" );
  spdlog::trace( fmt::runtime( "myVal = {:s}" ), myVal );
  EXPECT_EQ( myVal, "value" );
}
