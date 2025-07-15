#include "UIRenderer.h"

UIRenderer::UIRenderer(SDL_Renderer* renderer)
    : renderer(renderer){}

UIRenderer::~UIRenderer() {
}

void UIRenderer::drawRect(const SDL_Rect& rect, const Color& color, bool filled) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    if (filled)
        SDL_RenderFillRect(renderer, &rect);
    else
        SDL_RenderDrawRect(renderer, &rect);
}

void UIRenderer::drawTexture(SDL_Texture* texture, const SDL_Rect& destRect, SDL_Rect* srcRect) {
    if (!texture || !renderer) return;
    SDL_RenderCopy(renderer, texture, srcRect, &destRect);
}

void UIRenderer::drawText(const std::string& text, TTF_Font* font, const Color& color, const Vector2& pos)
{
    SDL_Color sdlColor = color.toSDL();

    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), sdlColor);
    if (!surface) return;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect dstRect = {
        static_cast<int>(pos.x),
        static_cast<int>(pos.y),
        surface->w,
        surface->h
    };

    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void UIRenderer::drawTextWithOutline(const std::string& text, TTF_Font* font,
                                     const Color& textColor, const Color& outlineColor,
                                     const Vector2& pos)
{
    SDL_Color textSDL = textColor.toSDL();
    SDL_Color outlineSDL = outlineColor.toSDL();

    SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), textSDL);
    SDL_Surface* outlineSurface = TTF_RenderText_Blended(font, text.c_str(), outlineSDL);
    if (!textSurface || !outlineSurface) return;

    SDL_Texture* textTex = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Texture* outlineTex = SDL_CreateTextureFromSurface(renderer, outlineSurface);
    if (!textTex || !outlineTex) {
        SDL_FreeSurface(textSurface);
        SDL_FreeSurface(outlineSurface);
        if (textTex) SDL_DestroyTexture(textTex);
        if (outlineTex) SDL_DestroyTexture(outlineTex);
        return;
    }

    SDL_Rect dst = {
        static_cast<int>(pos.x),
        static_cast<int>(pos.y),
        textSurface->w,
        textSurface->h
    };

    const int offset = 2;
    for (int dx = -offset; dx <= offset; ++dx) {
        for (int dy = -offset; dy <= offset; ++dy) {
            if (dx == 0 && dy == 0) continue;
            SDL_Rect outlineDst = { dst.x + dx, dst.y + dy, dst.w, dst.h };
            SDL_RenderCopy(renderer, outlineTex, nullptr, &outlineDst);
        }
    }

    SDL_RenderCopy(renderer, textTex, nullptr, &dst);

    SDL_FreeSurface(textSurface);
    SDL_FreeSurface(outlineSurface);
    SDL_DestroyTexture(textTex);
    SDL_DestroyTexture(outlineTex);
}
