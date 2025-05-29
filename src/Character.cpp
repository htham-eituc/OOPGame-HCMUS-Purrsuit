#include <iostream>
#include "Character.h"

Character::Character(SDL_Renderer* renderer, int x, int y, Map* map)
    : position(x, y), velocity(0, 0), lastFrameTime(SDL_GetTicks()), map(map) {
    destRect = { x, y, 64, 64 };
    srcRect = { 0, 0, frameWidth, frameHeight };
}

Character::~Character() {}

SDL_Rect Character::getCollisionBox(const Vector2& pos) const {
    int padLeft = 18;    // was 14
    int padRight = 0;    // was -4
    int padTop = 42;     // was 38
    int padBottom = -28; // was -32
    // Adjust this to taste (smaller = tighter collision)
    return {
        static_cast<int>(pos.x) + padLeft,
        static_cast<int>(pos.y) + padTop,
        frameWidth - padLeft - padRight,
        frameHeight - padTop - padBottom
    };
}

void Character::update(float deltaTime) {
    // Proposed new position
    Vector2 newPos = position + velocity * deltaTime;

    SDL_Rect futureBox = getCollisionBox(newPos);
    bool blocked = false;

    int tileSize = map->TILE_SIZE;
    int leftTile   = futureBox.x / tileSize;
    int rightTile  = (futureBox.x + futureBox.w - 1) / tileSize;
    int topTile    = futureBox.y / tileSize;
    int bottomTile = (futureBox.y + futureBox.h - 1) / tileSize;

    for (int ty = topTile; ty <= bottomTile && !blocked; ++ty) {
        for (int tx = leftTile; tx <= rightTile && !blocked; ++tx) {
            if (map->isCollidable(tx, ty)) {
                blocked = true;
            }
        }
    }

    if (!blocked) {
        position = newPos;
    }

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
    SDL_Rect colBox = getCollisionBox(position);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
    SDL_RenderDrawRect(renderer, &colBox);
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
