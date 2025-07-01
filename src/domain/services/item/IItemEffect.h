#pragma once

class Player;

class IItemEffect {
public:
    virtual void apply(Player& character) = 0;
    virtual void remove(Player& character) = 0;
    virtual ~IItemEffect() = default;
};