#include "ItemEffectHandler.h"
#include "Character.h"
#include "Services.h"
#include "Item.h"
#include "DataStruct.h"
#include <iostream>

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

void ItemEffectHandler::addItem(Item item, Player& player) {
    std::string itemName = item.getName();
    IItemEffect* effect = nullptr;

    if (itemName == "Catnip")
        effect = new SilentEffect();
    else if (itemName == "Apple")
        effect = new SpeedModifierEffect(1.25f);
    else if (itemName == "Milk")
        effect = new InvincibleEffect();
    else if (itemName == "Mushroom")
        effect = new SpeedModifierEffect(0.75f);
    else if (itemName == "Key") {
        core::soundEvent->emitSound({
            Vector2::fromRectCenter(item.getBounds()), 
            1000.0f,
            SDL_GetTicks() / 1000.0f
        });
    }

    if (effect) {
        effect->apply(player);
        uint32_t endTime = SDL_GetTicks() + 5000; // 5 seconds
        addEffect(endTime, effect);
    }
}
