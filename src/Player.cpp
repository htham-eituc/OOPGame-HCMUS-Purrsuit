#include "Player.h"
#include <SDL_image.h>
#include <iostream>

Player::Player(SDL_Renderer* renderer, int x, int y)
    : flip(false),
      frame(0), lastFrameTime(SDL_GetTicks())
{
    idleTexture = loadTexture(renderer, "assets/character/PirateCatIdle.png");
    walkTexture = loadTexture(renderer, "assets//character/PirateCatRun.png");
    position = Vector2(x, y);
    velocity = Vector2(0, 0);

    currentTexture = idleTexture;
    frameWidth = 32;
    frameHeight = 32;
    destRect = { x, y, 64, 64 };
    srcRect = { 0, 0, frameWidth, frameHeight };
    setAnimation(PlayerState::Idle);
}

Player::~Player() {
    SDL_DestroyTexture(idleTexture);
    SDL_DestroyTexture(walkTexture);
}

SDL_Texture* Player::loadTexture(SDL_Renderer* renderer, const char* path) {
    SDL_Surface* tempSurface = IMG_Load(path);
    if (!tempSurface) {
        std::cerr << "Failed to load image: " << path << " Error: " << IMG_GetError() << "\n";
        return nullptr;
    }
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    return tex;
}

const float moveSpeed = 200.0f; // pixels per second
void Player::move(const Uint8* keystate) {
    velocity = Vector2(0, 0);

    if (keystate[SDL_SCANCODE_W]) velocity.y = -1;
    if (keystate[SDL_SCANCODE_S]) velocity.y =  1;
    if (keystate[SDL_SCANCODE_A]) { velocity.x = -1; flip = true; }
    if (keystate[SDL_SCANCODE_D]) { velocity.x =  1; flip = false; }

    if (velocity.magnitude() > 0) {
        velocity = velocity.normalized() * moveSpeed;
}
}

void Player::handleEvent(const SDL_Event& e) {
    
}

void Player::update(float deltaTime) {
    position += velocity * deltaTime;
    destRect.x = static_cast<int>(position.x);
    destRect.y = static_cast<int>(position.y);

    // State detection
    if (velocity.x != 0 || velocity.y != 0) {
        setAnimation(PlayerState::Walking);
    } else {
        setAnimation(PlayerState::Idle);
    }

    // Animate
    Uint32 now = SDL_GetTicks();
    if (now - lastFrameTime >= frameDelay) {
        frame = (frame + 1) % frameCount;
        lastFrameTime = now;
    }

    srcRect.x = frame * frameWidth;
}

void Player::render(SDL_Renderer* renderer) {
    SDL_RendererFlip flipFlag = flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    SDL_RenderCopyEx(renderer, currentTexture, &srcRect, &destRect, 0, nullptr, flipFlag);
}

void Player::setAnimation(PlayerState newState) {
    if (newState == currentState) return;

    currentState = newState;
    frame = 0;
    lastFrameTime = SDL_GetTicks();

    switch (newState) {
        case PlayerState::Idle:
            currentTexture = idleTexture;
            frameCount = 3;
            break;
        case PlayerState::Walking:
            currentTexture = walkTexture;
            frameCount = 7;
            break;
    }
}