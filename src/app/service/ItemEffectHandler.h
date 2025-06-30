#pragma once
#include <vector>
#include <cstdint>
#include "IItemEffect.h"

struct ActiveItem {
    uint32_t endTime;
    IItemEffect* effect;
};

class ItemEffectHandler {
public:
    void addEffect(uint32_t endTime, IItemEffect* effect);
    void update(uint32_t currentTime, Player &character);
private:
    std::vector<ActiveItem> activeEffects;
};
