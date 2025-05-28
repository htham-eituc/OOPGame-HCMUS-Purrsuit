#include "Character.h"
#include <SDL_image.h>
#include <iostream>

Character::Character(SDL_Renderer* renderer, int x, int y)
    : position(x, y), velocity(0, 0), lastFrameTime(SDL_GetTicks()) {
    destRect = { x, y, 64, 64 };
    srcRect = { 0, 0, frameWidth, frameHeight };
}

Character::~Character() {}

void Character::update(float deltaTime) {
    position += velocity * deltaTime;
    destRect.x = static_cast<int>(position.x);
    destRect.y = static_cast<int>(position.y);

    Uint32 now = SDL_GetTicks();
    if (now - lastFrameTime >= frameDelay) {
        frame = (frame + 1) % frameCount;
        lastFrameTime = now;
    }

    srcRect.x = frame * frameWidth;
}

void Character::render(SDL_Renderer* renderer) {
    SDL_RenderCopyEx(renderer, currentTexture, &srcRect, &destRect, 0, nullptr, flipFlag);
}

SDL_Texture* Character::loadTexture(SDL_Renderer* renderer, const char* path) {
    SDL_Surface* tempSurface = IMG_Load(path);
    if (!tempSurface) {
        std::cerr << "Failed to load image: " << path << " Error: " << IMG_GetError() << "\n";
        return nullptr;
    }
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    return tex;
}

void Character::setAnimation(CharacterState newState) {
    if (newState != currentState) {
        currentState = newState;
        frame = 0;
        lastFrameTime = SDL_GetTicks();
    }
}