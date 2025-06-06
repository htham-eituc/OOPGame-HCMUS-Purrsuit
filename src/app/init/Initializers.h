#pragma once
#include <SDL.h>

namespace app::init {
    bool initSDL();
    bool initSDLImage();
    bool initSDLMixer();
    
    void registerCoreServices(SDL_Renderer* renderer);
    void loadAssets();  // e.g., title screen, UI sounds
}