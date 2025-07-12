#pragma once
#include <SDL.h>

namespace app::init {
    bool initSDL();
    
    void registerCoreServices(SDL_Renderer* renderer);
    void loadAssets();  
}