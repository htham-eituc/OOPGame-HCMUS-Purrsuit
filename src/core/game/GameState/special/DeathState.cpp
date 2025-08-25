#include "DeathState.h"
#include "Game.h"
#include "Services.h"
#include "Constants.h"

void DeathState::enter(Game* game) {
    core::audio->stopMusic();
    core::audio->stopAllSounds();
    core::audio->playSound(audio::zombie_eating, 0);
}

void DeathState::exit(Game* game) {
}

void DeathState::handleEvent(Game* game, const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
        game->startLevel(game->getStateMachine()->getPassLevel()); 
    }
}

void DeathState::update(Game* game, float deltaTime) {
}

void DeathState::render(Game* game) {
    SDL_Renderer* renderer = game->getRenderer();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Texture* deathTex = core::textures->getTexture(texture::death_screen);
    SDL_RenderCopy(renderer, deathTex, nullptr, nullptr);

    const char* restartText = "Press SPACE to Try Again";
    SDL_Color textColor = {255, 255, 255, 255};

    TTF_Font* popupFont = TTF_OpenFont("assets/fonts/Pixel12x10Mono-v1.1.0.ttf", 16);
    if (popupFont) {
        int textW, textH;
        TTF_SizeText(popupFont, restartText, &textW, &textH);

        int padding = 10;
        int popupWidth  = textW + (padding * 2);
        int popupHeight = textH + (padding * 2);

        int popupX = (SCREEN_WIDTH  - popupWidth) / 2;
        int popupY = (SCREEN_HEIGHT - popupHeight) / 2;

        SDL_Color bgColor = {20, 20, 20, 180};

        SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
        SDL_Rect popupBg = { popupX, popupY, popupWidth, popupHeight };
        SDL_RenderFillRect(renderer, &popupBg);

        SDL_SetRenderDrawColor(renderer, textColor.r, textColor.g, textColor.b, 255);
        SDL_RenderDrawRect(renderer, &popupBg);

        UILabel restartLabel(
            Vector2(popupX + padding, popupY + padding),
            Vector2(textW, textH),
            restartText,
            Color(textColor.r, textColor.g, textColor.b, textColor.a),
            Color(0, 0, 0, 255),
            popupFont
        );

        restartLabel.enableOutline(Color(0, 0, 0, 255));
        restartLabel.render(core::uiRenderer);

        TTF_CloseFont(popupFont);
    }
}
