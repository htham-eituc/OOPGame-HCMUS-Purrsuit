#include "DeathState.h"
#include "Game.h"
#include "Services.h"
#include "Constants.h"

void DeathState::enter(Game* game) {
    // Play death audio
    core::audio->stopMusic();
    core::audio->stopAllSounds();
    core::audio->playSound(audio::zombie_eating, 0);
}

void DeathState::exit(Game* game) {
    // Nothing special to clean up
}

void DeathState::handleEvent(Game* game, const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
        game->startLevel(4); 
    }
}

void DeathState::update(Game* game, float deltaTime) {
    // Death screen doesn't need updates
}

void DeathState::render(Game* game) {
    SDL_Renderer* renderer = game->getRenderer();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Texture* deathTex = core::textures->getTexture(texture::death_screen);
    SDL_RenderCopy(renderer, deathTex, nullptr, nullptr);

}