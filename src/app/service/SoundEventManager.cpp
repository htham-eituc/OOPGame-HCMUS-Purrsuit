#include <algorithm>
#include "SoundEventManager.h"
#include "SDL.h"

void SoundEventManager::emitSound(const SoundEvent& event) {
    activeEvents.push_back(event);
}

const std::vector<SoundEvent>& SoundEventManager::getActiveEvents() const {
    return activeEvents;
}

void SoundEventManager::update(float deltaTime) {
    float now = SDL_GetTicks() / 1000.0f;

    activeEvents.erase(
        std::remove_if(activeEvents.begin(), activeEvents.end(),
            [now, this](const SoundEvent& e) {
                return now - e.timestamp > eventLifetime;
            }),
        activeEvents.end()
    );
}

void SoundEventManager::clear() {
    activeEvents.clear();
}
