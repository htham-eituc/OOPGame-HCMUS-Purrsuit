#pragma once
#include "IItemEffect.h"
#include "Player.h"

class SilentEffect : public IItemEffect {
public:
    void apply(Player& character) override {
        character.addFlag("Silent");  
    }

    void remove(Player& character) override {
        character.removeFlag("Silent");
    }
};