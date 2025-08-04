#pragma once
#include <vector>
#include <cstdint>
#include <string>
#include "Item.h"
#include "IItemEffect.h"
#include "InvincibleEffect.h"
#include "SilentEffect.h"
#include "SpeedModifyEffect.h"

struct ActiveItem {
    uint32_t endTime;
    IItemEffect* effect;
};

class ItemEffectHandler {
public:
    void addEffect(uint32_t endTime, IItemEffect* effect);
    void update(uint32_t currentTime, Player &character);
    bool addItem(Item item, Player &character);
private:
    std::vector<ActiveItem> activeEffects;
};
