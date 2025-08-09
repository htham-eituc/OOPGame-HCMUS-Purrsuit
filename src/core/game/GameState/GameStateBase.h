#pragma once
#include <SDL.h>

class Game;

class GameStateBase {
public:
    virtual ~GameStateBase() = default;
    
    virtual void enter(Game* game) = 0;
    virtual void exit(Game* game) = 0;
    virtual void handleEvent(Game* game, const SDL_Event& event) = 0;
    virtual void update(Game* game, float deltaTime) = 0;
    virtual void render(Game* game) = 0;
};

