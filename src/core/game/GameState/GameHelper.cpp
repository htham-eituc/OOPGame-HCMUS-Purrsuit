#include "Game.h"

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
    if (currentState) {
        currentState->exit(this);  // Exit current state
    }
    currentState = std::move(newState);
    currentState->enter(this);     // Enter new state - HERE's where enter() is used!
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
    default:
        changeState(StateFactory::createCutscene1State());
        break;
    }
}

void Game::startLevel(int level)
{
    switch (level) {
        case 1: changeState(StateFactory::createLevel1State()); break;
        case 2: changeState(StateFactory::createLevel2State()); break;
        case 3: changeState(StateFactory::createLevel3State()); break;
        default: changeState(StateFactory::createLevel1State()); break;
    }
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