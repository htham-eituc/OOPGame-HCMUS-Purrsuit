#include "ItemEffectHandler.h"
#include "Character.h"

void ItemEffectHandler::addEffect(uint32_t endTime, IItemEffect* effect) {
    activeEffects.push_back({endTime, effect});
}

void ItemEffectHandler::update(uint32_t currentTime, Player &character) {
    for (auto it = activeEffects.begin(); it != activeEffects.end();) {
        if (currentTime >= it->endTime) {
            it->effect->remove(character);
            delete it->effect;
            it = activeEffects.erase(it);
        } else {
            ++it;
        }
    }
}
