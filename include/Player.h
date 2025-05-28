#pragma once
#include "DataStruct.h"
#include <SDL.h>

enum class PlayerState {
    Idle,
    Walking,
    // Add more later
};

class Player {
public:
    Player(SDL_Renderer* renderer, int x, int y);
    ~Player();

    void handleEvent(const SDL_Event& e);
    void update(float deltaTime);  
    void render(SDL_Renderer* renderer);
    void move(const Uint8* keystate);


private:
    SDL_Texture* idleTexture = nullptr;
    SDL_Texture* walkTexture = nullptr;
    //SDL_Texture* attackTexture = nullptr;

    SDL_Texture* currentTexture = nullptr;

    SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* path);

    PlayerState currentState = PlayerState::Idle;

    int frame = 0;
    int frameCount = 1;
    int frameWidth = 32;
    int frameHeight = 32;
    Uint32 lastFrameTime = 0;
    Uint32 frameDelay = 100;

    Vector2 position;
    Vector2 velocity;
    bool flip;

    SDL_Rect srcRect, destRect;

    void setAnimation(PlayerState newState);
};