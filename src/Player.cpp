// Player.cpp
#include "Player.h"
#include <iostream>
#include <SDL_image.h>

const float moveSpeed = 200.0f;

Player::Player(SDL_Renderer* renderer, int x, int y, Map *map)
    : Character(renderer, x, y, map)
{
    idleTexture = loadTexture(renderer, "assets/character/PirateCatIdle.png");
    walkTexture = loadTexture(renderer, "assets/character/PirateCatRun.png");

    currentTexture = idleTexture;
    setAnimation(CharacterState::Idle);
}

Player::~Player() {
    if (idleTexture) {
        SDL_DestroyTexture(idleTexture);
        idleTexture = nullptr;
    }
    if (walkTexture) {
        SDL_DestroyTexture(walkTexture);
        walkTexture = nullptr;
    }
}

void Player::move(const Uint8* keystate) {
    velocity = Vector2(0, 0);

    if (keystate[SDL_SCANCODE_W]) velocity.y -= 1;
    if (keystate[SDL_SCANCODE_S]) velocity.y += 1;
    if (keystate[SDL_SCANCODE_A]) { velocity.x -= 1; flipFlag = SDL_FLIP_HORIZONTAL; }
    if (keystate[SDL_SCANCODE_D]) { velocity.x += 1; flipFlag = SDL_FLIP_NONE; }
    if (velocity.magnitude() > 0)
        velocity = velocity.normalized() * moveSpeed;
}

void Player::handleEvent(const SDL_Event& e) {
    // Optional: handle player-specific input
}

void Player::update(float deltaTime) {
    if (velocity.x != 0 || velocity.y != 0) {
        setAnimation(CharacterState::Walking);
    } else {
        setAnimation(CharacterState::Idle);
    }
    Character::update(deltaTime); // reuse base logic
}

void Player::setAnimation(CharacterState newState) {
    if (newState == currentState) return;

    Character::setAnimation(newState);

    switch (newState) {
        case CharacterState::Idle:
            currentTexture = idleTexture;
            frameCount = 3;
            break;
        case CharacterState::Walking:
            currentTexture = walkTexture;
            frameCount = 7;
            break;
        default:
            currentTexture = idleTexture;
            frameCount = 3;
            break;
    }
}