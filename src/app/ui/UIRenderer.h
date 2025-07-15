#pragma once

#include <SDL.h>
#include <string>
#include "DataStruct.h"
#include "Color.h"
#include <SDL_ttf.h>

class UIRenderer
{
private:
    SDL_Renderer* renderer;
public:
    UIRenderer(SDL_Renderer* sdlRenderer);
    ~UIRenderer();

    void drawRect(const SDL_Rect& rect, const Color& color, bool filled = true);
    void drawTexture(SDL_Texture* texture, const SDL_Rect& destRect, SDL_Rect* srcRect = nullptr);
    void drawText(const std::string& text, TTF_Font* font, const Color& color, const Vector2& pos);
    void drawTextWithOutline(const std::string& text, TTF_Font* font,
                         const Color& textColor, const Color& outlineColor,
                         const Vector2& pos);
};