#include "UIButton.h"
#include "UIRenderer.h"

UIButton::UIButton(const SDL_Rect& rect, SDL_Texture* texture, const std::function<void()>& onClick)
    : UIElement(Vector2(rect.x, rect.y), Vector2(rect.w, rect.h), /*id=*/""),
      bounds(rect),
      texture(texture),
      onClick(onClick)
{}

UIButton::~UIButton() = default;

void UIButton::update(float deltaTime) {
    // Optional hover/press effects in future
}

void UIButton::render(UIRenderer* renderer) {
    if (!texture) return;
    renderer->drawTexture(texture, bounds);
}

bool UIButton::handleInput(const SDL_Event& event) {
    int mx, my;
    if (event.type == SDL_MOUSEMOTION) {
        mx = event.motion.x;
        my = event.motion.y;
        SDL_Point point = { mx, my };
        hovered = SDL_PointInRect(&point, &bounds);
    }
    else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        mx = event.button.x;
        my = event.button.y;
        SDL_Point point = { mx, my };
        if (SDL_PointInRect(&point, &bounds)) {
            pressed = true;
            return true;
        }
    }
    else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
        mx = event.button.x;
        my = event.button.y;
        SDL_Point point = { mx, my };
        if (pressed && SDL_PointInRect(&point, &bounds)) {
            pressed = false;
            if (onClick) onClick();  // Call the click handler
            return true;
        }
        pressed = false;
    }
    return false;
}
