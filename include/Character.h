#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "DataStruct.h"
#include "Map.h"

enum class CharacterState {
    Idle,
    Walking
};

class Character {
public:
    Character(SDL_Renderer* renderer, int x, int y, Map* map);
    virtual ~Character();

    virtual void move(const Uint8* keystate) {}
    virtual void handleEvent(const SDL_Event& e) {}
    virtual void update(float deltaTime);
    virtual void render(SDL_Renderer* renderer);

protected:
    SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* path);

    Vector2 position;
    Vector2 velocity;

    SDL_Texture* currentTexture = nullptr;
    SDL_Rect srcRect;
    SDL_Rect destRect;
    SDL_RendererFlip flipFlag = SDL_FLIP_NONE;

    int frameWidth = 32;
    int frameHeight = 32;
    int frameCount = 1;
    int frame = 0;
    Uint32 lastFrameTime = 0;
    Uint32 frameDelay = 100; // ms per frame
    
    CharacterState currentState = CharacterState::Idle;
    
    Map* map = nullptr;
    
    SDL_Rect getCollisionBox(const Vector2& pos) const;
    virtual void setAnimation(CharacterState newState);
};