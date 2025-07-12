#include <SDL.h>
#include "ZombieCat.h"
#include "Player.h"
#include "Camera.h" 
#include "MemoryUtils.h"
#include "Services.h"
#include "Constants.h"
#include "CollisionHandler.h"
#include "Map.h"
#include <iostream>

ZombieCat::ZombieCat(SDL_Renderer* renderer, int x, int y, Map* map, Player* player)
    : Character(renderer, x, y, map)
{
    this->player = player;
    currentTexture = core::textures->getTexture(texture::zombie_idle);
    setAnimation(CharacterState::Idle);
    transitionTo(ZombieState::Idle);
    pathFinder = new PathFinder(map->getCollisionMap(), map->getWidth(), map->getHeight());
}

ZombieCat::~ZombieCat() {
    //safeDelete(pathFinder);
}

void ZombieCat::update(float deltaTime) {
    zombieAI(deltaTime);
    Character::update(deltaTime);
    if (player && player->canBeKill()) {
        SDL_Rect zombieBox = getBounds();
        SDL_Rect playerBox = player->getBounds();

        if (CollisionHandler::checkCollision(zombieBox, playerBox)) {
            player->kill();  
        }
    }
}

void ZombieCat::zombieAI(float deltaTime) {
    const Vector2 zombieCenter = position + Vector2{
        static_cast<float>(frameWidth) / 2.0f,
        static_cast<float>(frameHeight) / 2.0f
    };

    switch (zombieState) {
        case ZombieState::Idle:
            // Check for sound
            for (auto& evt : core::soundEvent->getActiveEvents()) {
                float dist = (evt.position - zombieCenter).magnitude();
                if (dist <= soundAttractRadius) {
                    targetPos = evt.position;
                    transitionTo(ZombieState::InvestigatingSound);
                    return;
                }
            }

            // If near player and player is moving
            if (player->canBeHeard() && (player->getPosition() - zombieCenter).magnitude() < hearingRadius) {
                transitionTo(ZombieState::ChasingPlayer);
                return;
            }

            // Otherwise, wander slowly
            idleTimer += deltaTime;
            if (idleTimer > 1.0f) {
                idleTimer = 0;
                transitionTo(ZombieState::Wandering);
            }
            break;

        case ZombieState::Wandering: {
            wanderTimer += deltaTime;

            // Pick a direction only once per wandering phase
            if (wanderDirection == Vector2{0, 0}) {
                int dx = (rand() % 3) - 1; // -1, 0, or 1
                int dy = (rand() % 3) - 1;
                if (dx != 0 || dy != 0)
                    wanderDirection = Vector2{ static_cast<float>(dx), static_cast<float>(dy) }.normalized();
                else
                    wanderDirection = Vector2{1, 0}; // fallback
            }

            velocity = wanderDirection * 30.0f;

            // Predict the position after movement
            SDL_Rect futureBox = getCollisionBox(position + velocity * deltaTime);

            if (map && CollisionHandler::checkMapCollision(futureBox, *map)) {
                // If we hit a wall, cancel wandering
                velocity = {0, 0};
                wanderTimer = 0;
                wanderDirection = {0, 0};
                transitionTo(ZombieState::Idle);
                break;
            }

            // Stop wandering after a few seconds
            if (wanderTimer > 2.0f) {
                wanderTimer = 0;
                wanderDirection = {0, 0};
                velocity = {0, 0};
                transitionTo(ZombieState::Idle);
            }
            break;
        }
        
        case ZombieState::InvestigatingSound:
            pathTimer += deltaTime;

            if (path.empty() || pathTimer > 0.5f) {
                path = pathFinder->findPath(position, player->getPosition(), TILE_SIZE);
                currentPathIndex = 0;
                pathTimer = 0.0f;
            }

            if (!path.empty() && currentPathIndex < path.size()) {
                Vector2 target = path[currentPathIndex];
                Vector2 dir = (target - position).normalized();
                velocity = dir * 80.0f;

                if ((target - position).magnitude() < 5.0f)
                    currentPathIndex++;
            }

            if (!player->canBeHeard() || (player->getPosition() - position).magnitude() > hearingRadius) {
                transitionTo(ZombieState::Idle);
            }
            break;

        case ZombieState::ChasingPlayer:
            // Follow player
            pathTimer += deltaTime;

            if (path.empty() || pathTimer > 0.5f) {
                path = pathFinder->findPath(position, player->getPosition(), TILE_SIZE);
                currentPathIndex = 0;
                pathTimer = 0.0f;
            }

            if (!path.empty() && currentPathIndex < path.size()) {
                Vector2 target = path[currentPathIndex];
                Vector2 dir = (target - position).normalized();
                velocity = dir * 80.0f;

                if ((target - position).magnitude() < 5.0f)
                    currentPathIndex++;
            }

            if (!player->canBeHeard() || (player->getPosition() - position).magnitude() > hearingRadius) {
                transitionTo(ZombieState::Idle);
            }
            break;
    }

    // Set animation
    if (velocity.x != 0 || velocity.y != 0)
        setAnimation(CharacterState::Walking);
    else
        setAnimation(CharacterState::Idle);
}

void ZombieCat::transitionTo(ZombieState newState) {
    std::cout << "Zombie update: state = " << (int)zombieState << std::endl;
    if (zombieState == newState)
        return;

    zombieState = newState;
    velocity = {0, 0}; // Reset movement on transition

    switch (zombieState) {
        case ZombieState::Idle:
            setAnimation(CharacterState::Idle);
            break;

        case ZombieState::Wandering:
            setAnimation(CharacterState::Walking);
            break;

        case ZombieState::InvestigatingSound:
            // play alert animation or walking
            core::audio->playSound(audio::growl, 0);
            setAnimation(CharacterState::Walking);
            break;

        case ZombieState::ChasingPlayer:
            core::audio->playSound(audio::growl, 0);
            setAnimation(CharacterState::Walking);
            break;
    }
}

void ZombieCat::setAnimation(CharacterState newState) {
    if (newState == currentState) return;
    currentState = newState;

    switch (newState) {
        case CharacterState::Idle:
            currentTexture = core::textures->getTexture(texture::zombie_idle);
            frameCount = 3;
            break;
        case CharacterState::Walking:
            currentTexture = core::textures->getTexture(texture::zombie_walk);
            frameCount = 6;
            break;
    }

    frame = 0;
    lastFrameTime = SDL_GetTicks();
}
