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
