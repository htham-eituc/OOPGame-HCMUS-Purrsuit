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
        game->startLevel1(); // Restart level 1
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
    
    // Note: Death screen handles its own SDL_RenderPresent in your original code
    // You might want to move that logic here or handle it in the main render loop
}

// =================================================================
// WinState.cpp
// =================================================================
#include "WinState.h"
#include "Game.h"
#include "Services.h"
#include "Constants.h"

void WinState::enter(Game* game) {
    // Play victory music
    core::audio->stopMusic();
    core::audio->playMusic(audio::victory); // Assuming you have victory music
    
    int buttonWidth = SCREEN_WIDTH / 4;
    int buttonHeight = SCREEN_HEIGHT / 10;
    int centerX = SCREEN_WIDTH / 2 - buttonWidth / 2;
    
    // Create restart button
    SDL_Rect restartRect = { centerX, SCREEN_HEIGHT / 2, buttonWidth, buttonHeight };
    restartButton = std::make_shared<UIButton>(
        restartRect,
        core::textures->getTexture(texture::start_button), // Reuse start button texture
        [game]() { game->startLevel1(); }
    );
    core::uiInput->registerElement(restartButton);
    
    // Create main menu button
    SDL_Rect mainMenuRect = { centerX, SCREEN_HEIGHT / 2 + buttonHeight + 20, buttonWidth, buttonHeight };
    mainMenuButton = std::make_shared<UIButton>(
        mainMenuRect,
        core::textures->getTexture(texture::quit_button), // Or create a main menu texture
        [game]() { game->changeToTitleState(); }
    );
    core::uiInput->registerElement(mainMenuButton);
}

void WinState::exit(Game* game) {
    // Clean up win screen UI
    if (restartButton) core::uiInput->unregisterElement(restartButton);
    if (mainMenuButton) core::uiInput->unregisterElement(mainMenuButton);
    
    restartButton.reset();
    mainMenuButton.reset();
}

void WinState::handleEvent(Game* game, const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_SPACE:
                game->startLevel1(); // Restart game
                break;
            case SDLK_ESCAPE:
                game->changeToTitleState(); // Back to main menu
                break;
        }
    }
}

void WinState::update(Game* game, float deltaTime) {
    // Win screen doesn't need much updating
}

void WinState::render(Game* game) {
    SDL_Renderer* renderer = game->getRenderer();
    
    // Render win background (you'll need to create this texture)
    SDL_Texture* winTexture = core::textures->getTexture(texture::win_screen);
    SDL_RenderCopy(renderer, winTexture, nullptr, nullptr);
    
    // Render buttons
    if (restartButton) restartButton->render(core::uiRenderer);
    if (mainMenuButton) mainMenuButton->render(core::uiRenderer);
}