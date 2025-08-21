#include "TitleState.h"
#include "Game.h"
#include "Services.h"
#include "Constants.h"

void TitleState::enter(Game* game) {
    // Start title music
    if (!core::audio->isPlayingMusic()) {
        core::audio->playMusic(audio::title);
    }
    
    updateUILayout();
    
    // Create title-specific buttons
    startButton = std::make_shared<UIButton>(
        startButtonRect,
        core::textures->getTexture(texture::start_button),
        [game]() { game->startCutscenePlot(); }
    );
    core::uiInput->registerElement(startButton);
    
    loadButton = std::make_shared<UIButton>(
        loadButtonRect,
        core::textures->getTexture(texture::load_button),
        [game]() { game->loadGame("save.json"); }
    );
    core::uiInput->registerElement(loadButton);
}

void TitleState::exit(Game* game) {
    // Clean up title-specific UI
    if (startButton) core::uiInput->unregisterElement(startButton);
    if (loadButton) core::uiInput->unregisterElement(loadButton);
    
    startButton.reset();
    loadButton.reset();
}

void TitleState::handleEvent(Game* game, const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_9) {
        game->startCutscenePlot();
    }
}

void TitleState::update(Game* game, float deltaTime) {
    // Keep music playing
    if (!core::audio->isPlayingMusic()) {
        core::audio->playMusic(audio::title);
    }
}

void TitleState::render(Game* game) {
    SDL_Renderer* renderer = game->getRenderer();
    
    // Render title background
    SDL_Texture* titleTexture = core::textures->getTexture(texture::title_screen);
    SDL_RenderCopy(renderer, titleTexture, nullptr, nullptr);
    
    // Render buttons
    if (startButton) startButton->render(core::uiRenderer);
    if (loadButton) loadButton->render(core::uiRenderer);
}

void TitleState::updateUILayout() {
    int buttonWidth = SCREEN_WIDTH / 4;
    int buttonHeight = SCREEN_HEIGHT / 10;
    int centerX = SCREEN_WIDTH / 2 - buttonWidth / 2;
    
    startButtonRect = { centerX, SCREEN_HEIGHT / 2, buttonWidth, buttonHeight };
    loadButtonRect = { centerX, SCREEN_HEIGHT / 2 + buttonHeight + 20, buttonWidth, buttonHeight };
}