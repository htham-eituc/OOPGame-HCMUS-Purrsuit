#pragma once
#include "IItemEffect.h"
#include "Player.h"

class InvincibleEffect : public IItemEffect {
public:
    void apply(Player& character) override {
        character.addFlag("Invincible");
    }

    void remove(Player& character) override {
        character.removeFlag("Invincible");
    }
};
