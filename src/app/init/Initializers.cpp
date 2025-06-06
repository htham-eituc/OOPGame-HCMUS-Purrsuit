#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "Services.h"
#include "AudioManager.h"
#include "Constants.h"
#include "TextureManager.h"

namespace app::init {

    bool initSDL() {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            SDL_Log("SDL_Init failed: %s", SDL_GetError());
            return false;
        }

        if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
            SDL_Log("IMG_Init failed: %s", IMG_GetError());
            return false;
        }

        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            SDL_Log("SDL_mixer could not initialize! SDL_mixer Error: %s", Mix_GetError());
            return false;
        }
        return true;
    }

    void registerCoreServices(SDL_Renderer* renderer) {
        core::audio = new AudioManager();
        // core::textures = new TextureManager(renderer);
    }

    void loadAssets() {
        core::audio->loadMusic(audio::title, "assets/music/titleSoundtrack.mp3");
        core::audio->loadMusic(audio::lv1m, "assets/music/level1Soundtrack.mp3");
        core::audio->loadSound(audio::ping, "assets/music/itemPickupSound.wav");
        core::audio->loadSound(audio::grass, "assets/music/walkOnGrassSound.wav");
    }

}
