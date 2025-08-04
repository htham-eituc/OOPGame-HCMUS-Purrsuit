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

        if (TTF_Init() == -1) {
            SDL_Log("Failed to init SDL_ttf: %s", TTF_GetError());
            return false;
        }
        return true;
    }

    void registerCoreServices(SDL_Renderer* renderer) {
        core::audio = new AudioManager();
        core::textures = new TextureManager(renderer);
        core::soundEvent = new SoundEventManager();
        core::itemHandler = new ItemEffectHandler();
        core::uiInput = new UIInputHandler();
        core::uiRenderer = new UIRenderer(renderer);
        core::ui = new UIManager(core::uiRenderer);
    }

    void loadAssets() {
        // audio
        core::audio->loadMusic(audio::title, audio::title);
        core::audio->loadMusic(audio::cutscene_1_1, audio::cutscene_1_1);
        core::audio->loadMusic(audio::cutscene_1_2, audio::cutscene_1_2);
        core::audio->loadMusic(audio::cutscene_1_3, audio::cutscene_1_3);
        core::audio->loadMusic(audio::lv1m, audio::lv1m);
        core::audio->loadSound(audio::ping, audio::ping);
        core::audio->loadSound(audio::move, audio::move);
        core::audio->loadSound(audio::growl, audio::growl);
        core::audio->loadSound(audio::zombie_eating, audio::zombie_eating);
        core::audio->loadSound(audio::inventory, audio::inventory);
        core::audio->loadSound(audio::meow, audio::meow);
        
        // texture
        core::textures->loadTexture(texture::player_idle, texture::player_idle);
        core::textures->loadTexture(texture::player_walk, texture::player_walk);
        core::textures->loadTexture(texture::player_dead, texture::player_dead);
        core::textures->loadTexture(texture::zombie_idle, texture::zombie_idle);
        core::textures->loadTexture(texture::zombie_walk, texture::zombie_walk);
        core::textures->loadTexture(texture::zombie_stand_walk, texture::zombie_stand_walk);

        core::textures->loadTexture(texture::title_screen, texture::title_screen);
        core::textures->loadTexture(texture::start_button, texture::start_button);
        core::textures->loadTexture(texture::level1_exit_zone, texture::level1_exit_zone);
        core::textures->loadTexture(texture::cutscene_1_1, texture::cutscene_1_1);
        core::textures->loadTexture(texture::cutscene_1_2, texture::cutscene_1_2);
        core::textures->loadTexture(texture::cutscene_1_3, texture::cutscene_1_3);
        core::textures->loadTexture(texture::load_button, texture::load_button);
        core::textures->loadTexture(texture::save_button, texture::save_button);
        core::textures->loadTexture(texture::death_screen, texture::death_screen);
        core::textures->loadTexture(texture::resume_button, texture::resume_button);
        core::textures->loadTexture(texture::quit_button, texture::quit_button);
        core::textures->loadTexture(texture::wasd_onscreen, texture::wasd_onscreen);
        core::textures->loadTexture(texture::esc_onscreen, texture::esc_onscreen);
        core::textures->loadTexture(texture::mouse_default, texture::mouse_default);
        core::textures->loadTexture(texture::mouse_on_click, texture::mouse_on_click);
    }
}
