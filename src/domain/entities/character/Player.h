#pragma once 

#include <unordered_set>
#include "Character.h"

class Player : public Character {
private:
    std::unordered_set<std::string> activeFlags;
    float moveSpeed = 900.0f;

public:
    Player(SDL_Renderer* renderer, int x, int y, Map *map);
    ~Player();

    bool canBeHeard() const;
    bool canBeKill() const;
    bool isAlive() const;
    bool hasFlag(const std::string& flag) const;
    void addFlag(const std::string& flag);
    void removeFlag(const std::string& flag);

    void move(const Uint8* keystate) override;
    void handleEvent(const SDL_Event& e) override;
    void update(float deltaTime) override;
    void setAnimation(CharacterState newState) override;
    void setSpeed(const float &newSpeed); 
    void kill();
    
    float getSpeed() const;
    int getX() const;
    int getY() const;
};
