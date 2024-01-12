#include "engine/sdlaudio.h"

#include "engine/loggerfactory.h"


SFG::Engine::SdlAudio::SdlAudio( SdlEngine* sdlEngine )
    : logger_( SFG::Engine::LoggerFactory::get_logger( "Engine_SdlAudio" ) ),
      sdlEngine_( sdlEngine ),
      lastPlayedAudio_( "" ),
      sdlMixChunkBgm1_( nullptr ),
      sdlMixChunkBgm2_( nullptr ),
      sdlMixChunkSoundEffects_() {
  this->logger_->trace( fmt::runtime( "SdlAudio( sdlEngine = {:p} )" ), static_cast< void* >( sdlEngine ) );

  this->logger_->trace( fmt::runtime( "SdlAudio()~" ) );
}

SFG::Engine::SdlAudio::~SdlAudio() {
  this->logger_->trace( fmt::runtime( "~SdlAudio()" ) );

  if( this->sdlMixChunkBgm1_ ) {
    Mix_FreeChunk( this->sdlMixChunkBgm1_ );
  }
  this->sdlMixChunkBgm1_ = nullptr;
  if( this->sdlMixChunkBgm2_ ) {
    Mix_FreeChunk( this->sdlMixChunkBgm2_ );
  }
  this->sdlMixChunkBgm2_ = nullptr;
  for( auto chunk : this->sdlMixChunkSoundEffects_ ) {
    if( chunk ) {
      Mix_FreeChunk( chunk );
    }
  }
  this->sdlMixChunkSoundEffects_.clear();

  Mix_CloseAudio();

  this->logger_->trace( fmt::runtime( "~SdlAudio()~" ) );
}

// ############## DEBUG ############## DEBUG ############## DEBUG ##############

void SDLCALL sdl_mix_postmix( void* udata, uint8_t* stream, int len ) {
  for( int i = 0; i < len / 2; i++ ) {
    ( reinterpret_cast< int16_t* >( stream ) )[i] /= 2;
  }
}

// ############## DEBUG ############## DEBUG ############## DEBUG ##############

void SFG::Engine::SdlAudio::initialize_sdl_audio() {
  this->logger_->trace( fmt::runtime( "initialize_sdl_audio()" ) );

  SDL_AudioSpec audioSpec;
  int numPlaybackDevices = SDL_GetNumAudioDevices( 0 );
  this->logger_->trace( fmt::runtime( "audio playback devices ({:d}):" ), numPlaybackDevices );
  for( int i = 0; i < numPlaybackDevices; i++ ) {
    std::string deviceName = SDL_GetAudioDeviceName( i, 0 );
    if( SDL_GetAudioDeviceSpec( i, 0, &audioSpec ) != 0 ) {
      this->logger_->trace( fmt::runtime( "error when SDL_GetAudioDeviceSpec: {:s}" ), SDL_GetError() );
    }

    this->logger_->trace( fmt::runtime( R"(- "{:s}")" ), deviceName );
    this->logger_->trace( fmt::runtime( R"(  - freq:     {:d})" ), audioSpec.freq );
    this->logger_->trace( fmt::runtime( R"(  - format:   {:d})" ), audioSpec.format );
    this->logger_->trace( fmt::runtime( R"(    - bit size:   {:d})" ), audioSpec.format & SDL_AUDIO_MASK_BITSIZE );
    this->logger_->trace( fmt::runtime( R"(    - float:      {})" ), ( audioSpec.format & SDL_AUDIO_MASK_DATATYPE ) == SDL_AUDIO_MASK_DATATYPE );
    this->logger_->trace( fmt::runtime( R"(    - big endian: {})" ), ( audioSpec.format & SDL_AUDIO_MASK_ENDIAN ) == SDL_AUDIO_MASK_ENDIAN );
    this->logger_->trace( fmt::runtime( R"(    - signed:     {})" ), ( audioSpec.format & SDL_AUDIO_MASK_SIGNED ) == SDL_AUDIO_MASK_SIGNED );
    this->logger_->trace( fmt::runtime( R"(  - channels: {:d})" ), audioSpec.channels );
    this->logger_->trace( fmt::runtime( R"(  - silence:  {:d})" ), audioSpec.silence );
    this->logger_->trace( fmt::runtime( R"(  - samples:  {:d})" ), audioSpec.samples );
    this->logger_->trace( fmt::runtime( R"(  - padding:  {:d})" ), audioSpec.padding );
    this->logger_->trace( fmt::runtime( R"(  - size:     {:d})" ), audioSpec.size );
  }

  Mix_OpenAudio( 48000, AUDIO_S16, 2, 2048 );

  Mix_SetPostMix( sdl_mix_postmix, nullptr );

  this->logger_->trace( fmt::runtime( "initialize_sdl_audio()~" ) );
}

void SFG::Engine::SdlAudio::play_background_music( std::string const& path ) {
  this->logger_->trace( fmt::runtime( "play_background_music( path = \"{:s}\" )" ), path );
  if( this->lastPlayedAudio_ == path ) {
    return;
  }
  this->lastPlayedAudio_ = path;

  if( this->sdlMixChunkBgm1_ ) {
    Mix_FreeChunk( this->sdlMixChunkBgm1_ );
  }
  this->sdlMixChunkBgm1_ = Mix_LoadWAV( path.c_str() );

  if( this->sdlMixChunkBgm1_ == NULL ) {
    this->logger_->error( fmt::runtime( "play_background_music - error when Mix_LoadWAV: {:s}" ), SDL_GetError() );
  }

  if( Mix_FadeInChannel( 0, this->sdlMixChunkBgm1_, -1, 500 ) != 0 ) {
    this->logger_->error( fmt::runtime( "play_background_music - error when Mix_PlayChannel: {:s}" ), SDL_GetError() );
  }

  this->logger_->trace( fmt::runtime( "play_background_music()~" ) );
}
