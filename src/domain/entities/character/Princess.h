#pragma once
#include "Character.h"

class Princess : public Character {
private:
    float glowPulse; 
public:
    Princess(SDL_Renderer* renderer, int x, int y, Map* map);

    void move(const Uint8* keystate) override;
    void handleEvent(const SDL_Event& e) override;
    void update(float deltaTime) override;
    void renderCuteGlow(SDL_Renderer* renderer);
    void render(SDL_Renderer* renderer) override;
};
