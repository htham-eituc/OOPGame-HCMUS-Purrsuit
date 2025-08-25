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

    const char* restartText = "Press ENTER to Quit Game";
    SDL_Color textColor = {255, 255, 255, 255};

    TTF_Font* popupFont = TTF_OpenFont("assets/fonts/Pixel12x10Mono-v1.1.0.ttf", 13);
    if (popupFont) {
        int textW, textH;
        TTF_SizeText(popupFont, restartText, &textW, &textH);

        int padding = 8;
        int popupWidth  = textW + (padding * 2);
        int popupHeight = textH + (padding * 2);

        int popupX = (SCREEN_WIDTH - popupWidth) / 2;
        int bottomMargin = 60;
        int popupY = SCREEN_HEIGHT - popupHeight - bottomMargin;

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