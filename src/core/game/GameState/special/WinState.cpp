#include "WinState.h"
#include "Game.h"
#include "Services.h"
#include "Constants.h"

void WinState::enter(Game* game) {
    game->getTransitionManager()->reset();
    core::audio->stopMusic();
    core::audio->stopAllSounds();
    core::audio->playSound(audio::meow, 0);
    core::audio->playMusic(audio::lv11m, 0);
}

void WinState::exit(Game* game) {
}

void WinState::handleEvent(Game* game, const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
        game->setRunning(false); 
    }
}

void WinState::update(Game* game, float deltaTime) {
}

void WinState::render(Game* game) {
    SDL_Renderer* renderer = game->getRenderer();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Texture* winTex = core::textures->getTexture(texture::win_screen);
    SDL_RenderCopy(renderer, winTex, nullptr, nullptr);
}