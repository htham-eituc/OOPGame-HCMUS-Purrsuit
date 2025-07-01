#pragma once

#include "IItemEffect.h"
#include "Player.h"

class SpeedModifierEffect : public IItemEffect {
    float multiplier;

public:
    SpeedModifierEffect(float multiplier) : multiplier(multiplier) {}

    void apply(Player& character) override {
        character.setSpeed(character.getSpeed() * multiplier);
    }

    void remove(Player& character) override {
        character.setSpeed(character.getSpeed() / multiplier);
    }
};
