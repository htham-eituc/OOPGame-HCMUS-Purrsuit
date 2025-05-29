#pragma once 

#include "Character.h"
#include "Map.h"

class Player : public Character {
public:
    Player(SDL_Renderer* renderer, int x, int y, Map* map);
    ~Player();

    void move(const Uint8* keystate) override;
    void handleEvent(const SDL_Event& e) override;
    void update(float deltaTime) override;
    void setAnimation(CharacterState newState) override;

private:
    SDL_Texture* idleTexture = nullptr;
    SDL_Texture* walkTexture = nullptr;

    Map* map = nullptr;

    const float moveSpeed = 100.0f;
};