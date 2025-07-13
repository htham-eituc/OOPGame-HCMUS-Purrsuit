#pragma once

#include "Character.h"
#include "PathFinder.h"

class Player;

enum class ZombieState {
    Idle,
    Wandering,
    InvestigatingSound,
    ChasingPlayer
};

class ZombieCat : public Character {
private:
    Player* player = nullptr;

    ZombieState zombieState = ZombieState::Idle;
    ZombieState currentAnimationState = ZombieState::Idle;
    
    void setAnimation(ZombieState newState);
    void zombieAI(float deltaTime); 
    void transitionTo(ZombieState newState);

    float hearingRadius = 200.0f;   // close-range hearing
    float soundAttractRadius = 500.0f;

    PathFinder *pathFinder;
    std::vector<Vector2> path;
    int currentPathIndex = 0;
    float pathTimer = 0.0f;

    Vector2 targetPos;              // for A*
    Vector2 wanderDirection = {0, 0};
    float wanderTimer = 0;
    float idleTimer = 0;
    float lostPlayerTimer = 0;

    float speed = 50.0f;

public:
    ZombieCat(SDL_Renderer* renderer, int x, int y, Map* map, Player *player);
    ~ZombieCat();

    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer) override;
};
