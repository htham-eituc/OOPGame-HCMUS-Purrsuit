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

void ZombieCat::render(SDL_Renderer *renderer) {
        // Call base render
    Character::render(renderer);

    // 🔍 Debug draw path
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255); // Cyan

    for (const Vector2& p : path) {
        SDL_Rect dot = {
            static_cast<int>(p.x - 2),
            static_cast<int>(p.y - 2),
            4, 4
        };
        SDL_Rect camdes = Camera::ToCamView(dot);
        SDL_RenderFillRect(renderer, &camdes);
    }
}

void ZombieCat::zombieAI(float deltaTime) {
    const Vector2 zombieCenter = Vector2::fromRectCenter(getBounds());

    switch (zombieState) {
        case ZombieState::Idle: {
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
        }
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

        case ZombieState::InvestigatingSound: {
            pathTimer += deltaTime;

            // Check if we're close enough to the target position to stop investigating
            float distanceToFinalTarget = (targetPos - zombieCenter).magnitude();
            if (distanceToFinalTarget <= TILE_SIZE) {
                // Reached the sound source, go back to idle
                velocity = {0, 0};
                transitionTo(ZombieState::Idle);
                break;
            }

            // Generate initial path or recalculate if needed
            if (path.empty()) {
                path = pathFinder->findPath(zombieCenter, targetPos, TILE_SIZE);
                currentPathIndex = 0;
                pathTimer = 0.0f;
            }

            // Follow the path
            if (!path.empty() && currentPathIndex < path.size()) {
                Vector2 target = path[currentPathIndex];
                Vector2 toTarget = target - zombieCenter;
                float distanceToWaypoint = toTarget.magnitude();

                // Use a larger threshold for waypoint reached (about half a tile)
                if (distanceToWaypoint > TILE_SIZE / 2.0f) {
                    Vector2 dir = toTarget.normalized();
                    velocity = dir * 80.0f;
                } else {
                    // Move to next waypoint
                    currentPathIndex++;
                    if (currentPathIndex >= path.size()) {
                        // Completed the path, check if we're at the target
                        if (distanceToFinalTarget <= TILE_SIZE) {
                            velocity = {0, 0};
                            transitionTo(ZombieState::Idle);
                        } else {
                            // Path completed but not at target, recalculate
                            path.clear();
                        }
                    }
                }
            } else {
                // No path available, go back to idle
                velocity = {0, 0};
                transitionTo(ZombieState::Idle);
            }

            // Timeout - if investigating too long, give up
            if (pathTimer > 15.0f) {
                velocity = {0, 0};
                transitionTo(ZombieState::Idle);
            }
            break;
        }
        case ZombieState::ChasingPlayer: {
            pathTimer += deltaTime;

            // Check if we should stop chasing 
            Vector2 playerCenter = Vector2::fromRectCenter(player->getBounds());
            float distanceToPlayer = (playerCenter - zombieCenter).magnitude();
            
            // Update lost player timer based on current conditions
            if (player->canBeHeard() && distanceToPlayer <= hearingRadius) {
                // Player is audible and in range, reset lost timer
                lostPlayerTimer = 0.0f;
            } else {
                // Player is not audible or too far, start counting
                lostPlayerTimer += deltaTime;
            }
            
            // Only stop chasing after losing the player for a while
            if (lostPlayerTimer > 1.7f) { // 1.7 second delay before giving up
                velocity = {0, 0};
                transitionTo(ZombieState::Idle);
                break;
            }

            // If we're very close to the player, move directly toward them (no pathfinding)
            if (distanceToPlayer <= TILE_SIZE) {
                Vector2 directToPlayer = (playerCenter - zombieCenter).normalized();
                
                // Adjust speed based on distance for smoother approach
                float speedMultiplier = 1.0f;
                if (distanceToPlayer <= TILE_SIZE*0.5) {
                    // Very close - slow down for precision
                    speedMultiplier = 0.7f;
                } else if (distanceToPlayer <= TILE_SIZE * 0.75f) {
                    // Close - moderate speed
                    speedMultiplier = 0.85f;
                }
                
                velocity = directToPlayer * (120.0f * speedMultiplier);
                
                // Predict collision and make sure we can move there
                SDL_Rect futureBox = getCollisionBox(position + velocity * deltaTime);
                if (map && CollisionHandler::checkMapCollision(futureBox, *map)) {
                    // Can't move directly, fall back to pathfinding
                    velocity = {0, 0};
                    // Force path recalculation
                    path.clear();
                }
            } else {
                // Use pathfinding for longer distances
                // Recalculate path periodically to follow moving player
                if (path.empty() || pathTimer > 0.5f) {
                    path = pathFinder->findPath(zombieCenter, playerCenter, TILE_SIZE);
                    currentPathIndex = 0;
                    pathTimer = 0.0f;
                }

                // Follow the path
                if (!path.empty() && currentPathIndex < path.size()) {
                    Vector2 target = path[currentPathIndex];
                    Vector2 toTarget = target - zombieCenter;
                    float distanceToWaypoint = toTarget.magnitude();

                    // Use a larger threshold for waypoint reached (about half a tile)
                    if (distanceToWaypoint > TILE_SIZE / 2.0f) {
                        Vector2 dir = toTarget.normalized();
                        velocity = dir * 100.0f; // Slightly faster when chasing
                    } else {
                        // Move to next waypoint
                        currentPathIndex++;
                        if (currentPathIndex >= path.size()) {
                            // Completed current path, force recalculation to get closer
                            path.clear();
                        }
                    }
                } else {
                    // No path available, try to recalculate
                    path = pathFinder->findPath(zombieCenter, playerCenter, TILE_SIZE);
                    currentPathIndex = 0;
                }
            }
            break;
        }
    }

    // Set animation based on movement
    if (velocity.x != 0 || velocity.y != 0)
        setAnimation(CharacterState::Walking);
    else
        setAnimation(CharacterState::Idle);
}

void ZombieCat::transitionTo(ZombieState newState) {
    if (zombieState == newState)
        return;

    zombieState = newState;
    path.clear();              
    currentPathIndex = 0;     
    pathTimer = 0.0f;   
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
