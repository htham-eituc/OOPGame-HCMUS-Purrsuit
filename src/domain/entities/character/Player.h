#pragma once
#include "Character.h"
#include <set>
#include <string>

class Player : public Character {
private:
    std::set<std::string> activeFlags;
    float glowPulse; 
    float moveSpeed;
    void renderInvincibilityGlow(SDL_Renderer* renderer);

public:
    Player(SDL_Renderer* renderer, int x, int y, Map *map);
    ~Player();

    void move(const Uint8* keystate);
    void handleEvent(const SDL_Event& e);
    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer) override;
    void setAnimation(CharacterState newState) override;
    
    // Flag management
    bool hasFlag(const std::string &flag) const;
    void addFlag(const std::string &flag);
    void removeFlag(const std::string &flag);
    
    // Player state queries
    bool canBeHeard() const;
    bool canBeKill() const;
    bool isAlive() const;
    
    // Player controls
    void setSpeed(const float &newSpeed);
    void kill();
    
    // Getters
    float getSpeed() const;
    int getX() const;
    int getY() const;
};