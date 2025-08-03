#pragma once
#include <string>
#include <SDL.h>

struct TransitionZone {
    SDL_Rect bounds;
    std::string toZone;
    std::string instruction; 
    std::string entering; 
    std::string requiredItem; 
    
    // Visual effect properties
    float pulseTimer = 0.0f;
    float glowIntensity = 0.0f;
    bool playerNear = false;
};