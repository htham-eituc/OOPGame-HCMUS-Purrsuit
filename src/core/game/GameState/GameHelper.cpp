#include "Game.h"
#include <iostream>

SDL_Renderer* Game::getRenderer() const {
    return renderer;
}

Camera *Game::getCamera() const {
    return camera;
}

Inventory *Game::getInventory() const {
    return inventory;
}

TransitionManager *Game::getTransitionManager() const {
    return transitionManager;
}

InventoryTextureManager *Game::getInventoryTextureManager() const {
    return inventoryTextureManager;
}

GameSave *Game::getGameSave() {
    return &gameSave;
}

GameStateMachine *Game::getStateMachine() const {
    return stateMachine;
}

void Game::setRunning(bool state) {
    running = state;
}

float Game::calculateDeltaTime(Uint32& lastTime) {
    Uint32 currentTime = SDL_GetTicks();
    float deltaTime = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;
    return deltaTime;
}

void Game::changeState(std::unique_ptr<GameStateBase> newState) {
    pendingState = std::move(newState);
    blockAllInput = true;
    stateChangeTimer = SDL_GetTicks();
}

void Game::performStateChange() {
    if (!pendingState) return;
    isChangingState = true;
    if (currentState) currentState->exit(this);
    
    SDL_Event clearEvent;
    while (SDL_PollEvent(&clearEvent)) {
        if (clearEvent.type == SDL_QUIT) {
            running = false;
        }
    }
    
    currentState = std::move(pendingState);
    pendingState.reset();
    
    if (currentState) {
        currentState->enter(this);
    }
    
    isChangingState = false;
}

void Game::renderLoadingScreen() {
    SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);
    SDL_RenderClear(renderer);
    
    // Optional: Simple loading text
    TTF_Font* font = TTF_OpenFont("assets/fonts/Pixel12x10Mono-v1.1.0.ttf", 24);
    if (font) {
        UILabel loadingLabel(
            Vector2(SCREEN_WIDTH/2 - 60, SCREEN_HEIGHT/2),
            Vector2(120, 30),
            "Loading...",
            Color(255, 255, 255, 255),
            Color(0, 0, 0, 255),
            font
        );
        loadingLabel.render(core::uiRenderer);
        TTF_CloseFont(font);
    }
    
    SDL_RenderPresent(renderer);
}

void Game::startCutscenePlot() {
    GameState gameState = stateMachine->getCurrentState();
    switch (gameState) {
    case GameState::TITLE:
        changeState(StateFactory::createCutscene1State());
        break;
    case GameState::LEVEL1: 
        changeState(StateFactory::createCutscene1State()); 
        break;
    case GameState::LEVEL3: 
        changeState(StateFactory::createCutscene2State()); 
        break;
    case GameState::LEVEL9:
        changeState(StateFactory::createCutscene3State()); 
        break;
    default:
        changeState(StateFactory::createCutscene1State());
        break;
    }
}

void Game::startLevel(int level = 0) {
    if(level == 0) 
        level = static_cast<int>(stateMachine->getCurrentState()) - static_cast<int>(GameState::LEVEL1) + 1;
    if(level > 11) {
        changeState(StateFactory::createWinState());
        return;
    }
    changeState(StateFactory::createLevelState(level));
}

void Game::renderCursor() {
    SDL_ShowCursor(SDL_DISABLE);

    int mouseXWindow, mouseYWindow;
    SDL_GetMouseState(&mouseXWindow, &mouseYWindow);
    float mouseX, mouseY;
    SDL_RenderWindowToLogical(renderer, mouseXWindow, mouseYWindow, &mouseX, &mouseY);

    SDL_Texture* cursorTex;
    SDL_Rect srcRect = { 0, 0, 21, 21 };

    if (mouseClicked) {
        cursorTex = core::textures->getTexture(texture::mouse_on_click);
        srcRect.x = clickCursorFrame * 21;
    } else {
        cursorTex = core::textures->getTexture(texture::mouse_default);
    }

    SDL_Rect dstRect = { 
        static_cast<int>(mouseX) - 3, 
        static_cast<int>(mouseY) - 3, 
        21, 21 
    };
    SDL_RenderCopy(renderer, cursorTex, &srcRect, &dstRect);
}

void Game::updateCursorAnimation(float deltaTime) {
    if (mouseClicked) {
        clickCursorTimer += deltaTime;
        clickCursorAnimTimer += deltaTime;

        if (clickCursorAnimTimer >= frameDuration) {
            clickCursorAnimTimer = 0.0f;
            clickCursorFrame++;
            if (clickCursorFrame > 3) clickCursorFrame = 3;
        }

        if (clickCursorTimer >= clickCursorDuration) {
            mouseClicked = false;
            clickCursorFrame = 0;
            clickCursorAnimTimer = 0.0f;
        }
    }
}