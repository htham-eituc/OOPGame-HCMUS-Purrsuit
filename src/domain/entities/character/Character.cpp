#include <iostream>
#include "Character.h"
#include "CollisionHandler.h"
#include "Camera.h"

Character::Character(SDL_Renderer* renderer, int x, int y, Map* map)
    : position(x, y), velocity(0, 0), lastFrameTime(SDL_GetTicks()), map(map) {
    destRect = { x, y, 64, 64 };
    srcRect = { 0, 0, frameWidth, frameHeight };
}

Character::~Character() {}

SDL_Rect Character::getCollisionBox(const Vector2& pos) const {
    int padLeft = 20;
    int padRight = -12;
    int padTop = 48;
    int padBottom = -32;
    // Adjust this to taste (smaller = tighter collision)
    return {
        static_cast<int>(pos.x) + padLeft,
        static_cast<int>(pos.y) + padTop,
        frameWidth - padLeft - padRight,
        frameHeight - padTop - padBottom
    };
}

bool Character::isMoving() const {
    if (velocity.x != 0 || velocity.y != 0) return true;
    return false;
}

SDL_Rect Character::getBounds() const {
    return getCollisionBox(position);
}

Vector2 Character::getPosition() const {
    return position;
}


void Character::update(float deltaTime) {
    if(velocity.x < 0) flipFlag = SDL_FLIP_HORIZONTAL;
    if(velocity.x > 0) flipFlag = SDL_FLIP_NONE;
    Vector2 fullMove = velocity * deltaTime;
    Vector2 newPos = position;

    bool movedX = false;
    bool movedY = false;

    // Try horizontal movement
    if (velocity.x != 0.0f) {
        Vector2 tryX = { position.x + fullMove.x, position.y };
        SDL_Rect boxX = getCollisionBox(tryX);

        if (!CollisionHandler::checkMapCollision(boxX, *map)) {
            newPos.x = tryX.x;
            movedX = true;
        }
    }

    // Try vertical movement
    if (velocity.y != 0.0f) {
        Vector2 tryY = { newPos.x, position.y + fullMove.y }; // use updated X pos
        SDL_Rect boxY = getCollisionBox(tryY);

        if (!CollisionHandler::checkMapCollision(boxY, *map)) {
            newPos.y = tryY.y;
            movedY = true;
        }
    }

    position = newPos;

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
    SDL_Rect camDes = Camera::ToCamView(colBox);
    SDL_RenderDrawRect(renderer, &camDes);
    camDes = Camera::ToCamView(destRect);
    SDL_RenderCopyEx(renderer, currentTexture, &srcRect, &camDes, 0, nullptr, flipFlag);
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
