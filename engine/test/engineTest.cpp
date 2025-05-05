#include <SFG/Own2dEngine/Engine/audioManager.h>
#include <gtest/gtest.h>

TEST( Engine, Example ) {
  EXPECT_TRUE( true );
}

TEST( Engine, AudioDownSample ) {
  std::vector< float > origSamples{
      0, 1.0f, 0, -1.0f, 0, 1.0f,
      0, -1.0f, 0, 1.0f, 0, -1.0f,
      0, 1.0f, 0, -1.0f, 0, 1.0f,
      0, -1.0f, 0, 1.0f, 0, -1.0f,
      0, 1.0f, 0, -1.0f, 0, 1.0f,
      0, -1.0f, 0,
  };
  std::vector< float > newSamples;

  SFG::Own2dEngine::Engine::AudioManager::Resample( origSamples, 44100, 1, 16000, newSamples );

  EXPECT_EQ( 12, newSamples.size() );
}

TEST( Engine, AudioUpSample ) {
  std::vector< float > origSamples{
      0,
      1.0f,
      0,
      -1.0f,
      0,
  };
  std::vector< float > newSamples;

  SFG::Own2dEngine::Engine::AudioManager::Resample( origSamples, 16000, 1, 44100, newSamples );

  EXPECT_EQ( 14, newSamples.size() );
}

TEST( Engine, AudioUpSampleExtreme ) {
  std::vector< float > origSamples{
      -1.0f,
      0,
      1.0f,
  };
  std::vector< float > newSamples;

  SFG::Own2dEngine::Engine::AudioManager::Resample( origSamples, 3 * 400, 1, 44100, newSamples );

  EXPECT_EQ( 111, newSamples.size() );
}
