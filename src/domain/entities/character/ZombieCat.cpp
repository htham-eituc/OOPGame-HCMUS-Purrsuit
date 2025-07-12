#include "ZombieCat.h"
#include "Player.h"
#include "Camera.h" 
#include "MemoryUtils.h"
#include "Services.h"
#include "Constants.h"
#include "CollisionHandler.h"
#include "Map.h"

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
    safeDelete(pathFinder);
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
    
    switch (state) {
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
            if (idleTimer > 3.0f) {
                idleTimer = 0;
                transitionTo(ZombieState::Wandering);
            }
            break;

        case ZombieState::Wandering:
            // Random slow move
            wanderTimer += deltaTime;
            velocity.x = (rand() % 2 == 0 ? -20 : 20);

            if (wanderTimer > 2.0f) {
                wanderTimer = 0;
                velocity.x = 0;
                transitionTo(ZombieState::Idle);
            }
            break;

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
            // Use A* or simple direct walk to targetPos
            // If reached or timeout, return to idle/wandering
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
    if (state == newState)
        return;

    state = newState;
    velocity = {0, 0}; // Reset movement on transition

    switch (state) {
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
