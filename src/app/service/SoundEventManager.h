#pragma once
#include <vector>
#include "DataStruct.h"
#include <string>

struct SoundEvent {
    Vector2 position;
    float radius; // how far it can be heard
    float timestamp; // to allow fade or lifetime
};

class SoundEventManager {
public:
    void emitSound(const SoundEvent& event);
    const std::vector<SoundEvent>& getActiveEvents() const;

    void update(float deltaTime);
    void clear();

private:
    std::vector<SoundEvent> activeEvents;
    float eventLifetime = 3.0f; // seconds
};
