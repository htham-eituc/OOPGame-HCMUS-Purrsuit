#include "Player.h"
#include "MemoryUtils.h"
#include "Services.h"
#include "Constants.h"
#include "Camera.h"
#include <iostream>
#include <SDL_image.h>
#include <cmath>

Player::Player(SDL_Renderer* renderer, int x, int y, Map *map)
    : Character(renderer, x, y, map), glowPulse(0.0f) {
    currentTexture = core::textures->getTexture(texture::player_idle);
    setAnimation(CharacterState::Idle);
    moveSpeed = 500.0f;
}

Player::~Player() {
}

void Player::move(const Uint8* keystate) {
    velocity = Vector2(0, 0);

    if (keystate[SDL_SCANCODE_W]) velocity.y -= 1;
    if (keystate[SDL_SCANCODE_S]) velocity.y += 1;
    if (keystate[SDL_SCANCODE_A]) velocity.x -= 1; 
    if (keystate[SDL_SCANCODE_D]) velocity.x += 1;  
    if (velocity.magnitude() > 0)
        velocity = velocity.normalized() * moveSpeed;
}

void Player::handleEvent(const SDL_Event& e) {
    // Optional: handle player-specific input
}

bool Player::canBeHeard() const {
    return isMoving() && !hasFlag("Silent");
}

bool Player::canBeKill() const {
    return !hasFlag("Invincible");
}

bool Player::isAlive() const {
    return !hasFlag("Killed");
}

bool Player::hasFlag(const std::string &flag) const {
    return activeFlags.find(flag) != activeFlags.end();
}

void Player::addFlag(const std::string &flag) {
    activeFlags.insert(flag);
}

void Player::removeFlag(const std::string &flag) {
    activeFlags.erase(flag);
}

void Player::update(float deltaTime) {
    if (isMoving()) {
        setAnimation(CharacterState::Walking);
        if(canBeHeard()) core::audio->playSound(audio::move);
        else core::audio->stopSound(audio::move);
    } else {
        setAnimation(CharacterState::Idle);
        core::audio->stopSound(audio::move);
    }
    
    if (hasFlag("Invincible")) {
        glowPulse += deltaTime * 4.0f; 
        if (glowPulse > 2.0f * M_PI) {
            glowPulse -= 2.0f * M_PI;
        }
    }
    
    Character::update(deltaTime); // reuse base logic
}

void Player::render(SDL_Renderer* renderer) {
    if (hasFlag("Invincible")) 
        renderInvincibilityGlow(renderer);

    
    Character::render(renderer);
}

void Player::renderInvincibilityGlow(SDL_Renderer* renderer) {
    float pulseValue = (sin(glowPulse) + 1.0f) * 0.2f; // 0 to 1
    Uint8 alpha = static_cast<Uint8>(60 + pulseValue * 80); // 60 to 140
    
    SDL_Rect collisionBox = getCollisionBox(position);
    
    int centerX = collisionBox.x + collisionBox.w / 2;
    int centerY = collisionBox.y + collisionBox.h / 2;
    
    SDL_Rect centerRect = {centerX, centerY, 1, 1};
    SDL_Rect camCenter = Camera::ToCamView(centerRect);
    
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);
    
    // Golden glow color
    Uint8 r = 255, g = 215, b = 100;
    
    for (int layer = 5; layer >= 1; layer--) {
        int radius = layer * 12 + static_cast<int>(pulseValue * 6); // Pulsing radius
        Uint8 layerAlpha = alpha / (layer + 1); // Decrease alpha for outer layers
        
        SDL_SetRenderDrawColor(renderer, r, g, b, layerAlpha);
        
        // Draw filled circle using multiple horizontal lines
        for (int y = -radius; y <= radius; y++) {
            int width = static_cast<int>(sqrt(radius * radius - y * y) * 2);
            if (width > 0) {
                SDL_Rect line = {
                    camCenter.x - width / 2,
                    camCenter.y + y,
                    width,
                    1
                };
                SDL_RenderFillRect(renderer, &line);
            }
        }
    }
    
    // Add some sparkle effect
    for (int i = 0; i < 8; i++) {
        float angle = (glowPulse * 1.0f + i * M_PI / 4.0f);
        int sparkleRadius = 40 + static_cast<int>(pulseValue * 10);
        int sparkleX = camCenter.x + static_cast<int>(cos(angle) * sparkleRadius);
        int sparkleY = camCenter.y + static_cast<int>(sin(angle) * sparkleRadius);
        
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, static_cast<Uint8>(alpha * 0.8f));
        SDL_Rect sparkle = {sparkleX - 2, sparkleY - 2, 4, 4};
        SDL_RenderFillRect(renderer, &sparkle);
    }
    
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
}

void Player::setAnimation(CharacterState newState) {
    if (newState == currentState) return;

    Character::setAnimation(newState);

    switch (newState) {
        case CharacterState::Idle:
            currentTexture = core::textures->getTexture(texture::player_idle);
            frameCount = 3;
            break;
        case CharacterState::Walking:
            currentTexture = core::textures->getTexture(texture::player_walk);
            frameCount = 7;
            break;
        default:
            currentTexture = core::textures->getTexture(texture::player_idle);
            frameCount = 3;
            break;
    }
}

void Player::setSpeed(const float &newSpeed) {
    moveSpeed = newSpeed;
}

void Player::kill() {
    if(!isAlive()) return;
    addFlag("Killed");

    velocity = {0, 0};
    setAnimation(CharacterState::Idle);
    currentTexture = core::textures->getTexture(texture::player_dead); 

    std::cout << "Player has died!\n";
}

float Player::getSpeed() const {
    return moveSpeed;
}

int Player::getX() const { return position.x; }
int Player::getY() const { return position.y; }