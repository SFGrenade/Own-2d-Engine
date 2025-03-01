#include <SFG/Own2dEngine/Engine/audioManager.h>
#include <gtest/gtest.h>

TEST( Engine, Example ) {
  EXPECT_TRUE( true );
}

TEST( Engine, AudioDownSample ) {
  std::vector< int16_t > origSamples{
      0, std::numeric_limits< int16_t >::max(), 0, std::numeric_limits< int16_t >::min(), 0, std::numeric_limits< int16_t >::max(),
      0, std::numeric_limits< int16_t >::min(), 0, std::numeric_limits< int16_t >::max(), 0, std::numeric_limits< int16_t >::min(),
      0, std::numeric_limits< int16_t >::max(), 0, std::numeric_limits< int16_t >::min(), 0, std::numeric_limits< int16_t >::max(),
      0, std::numeric_limits< int16_t >::min(), 0, std::numeric_limits< int16_t >::max(), 0, std::numeric_limits< int16_t >::min(),
      0, std::numeric_limits< int16_t >::max(), 0, std::numeric_limits< int16_t >::min(), 0, std::numeric_limits< int16_t >::max(),
      0, std::numeric_limits< int16_t >::min(), 0,
  };
  std::vector< int16_t > newSamples;

  SFG::Own2dEngine::Engine::AudioManager::Resample( origSamples, 44100, 16000, newSamples );

  EXPECT_EQ( 12, newSamples.size() );
}

TEST( Engine, AudioUpSample ) {
  std::vector< int16_t > origSamples{
      0,
      std::numeric_limits< int16_t >::max(),
      0,
      std::numeric_limits< int16_t >::min(),
      0,
  };
  std::vector< int16_t > newSamples;

  SFG::Own2dEngine::Engine::AudioManager::Resample( origSamples, 16000, 44100, newSamples );

  EXPECT_EQ( 14, newSamples.size() );
}

TEST( Engine, AudioUpSampleExtreme ) {
  std::vector< int16_t > origSamples{
      std::numeric_limits< int16_t >::min(),
      0,
      std::numeric_limits< int16_t >::max(),
  };
  std::vector< int16_t > newSamples;

  SFG::Own2dEngine::Engine::AudioManager::Resample( origSamples, 3 * 400, 44100, newSamples );

  EXPECT_EQ( 111, newSamples.size() );
}
