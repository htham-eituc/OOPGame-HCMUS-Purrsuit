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