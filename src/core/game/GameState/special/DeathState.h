#pragma once
#include "GameStateBase.h"

class DeathState : public GameStateBase {
public:
    void enter(Game* game) override;
    void exit(Game* game) override;
    void handleEvent(Game* game, const SDL_Event& event) override;
    void update(Game* game, float deltaTime) override;
    void render(Game* game) override;
};

