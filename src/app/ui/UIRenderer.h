#pragma once

#include <SDL.h>
#include <string>
#include "DataStruct.h"
#include "Color.h"

class UIRenderer
{
private:
    SDL_Renderer* renderer;
public:
    UIRenderer(SDL_Renderer* sdlRenderer);
    ~UIRenderer();

    void drawRect(const SDL_Rect& rect, const Color& color, bool filled = true);
    void drawTexture(SDL_Texture* texture, const SDL_Rect& destRect, SDL_Rect* srcRect = nullptr);
};