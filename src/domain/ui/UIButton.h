#pragma once

#include <string>
#include <functional>
#include "UIElement.h"
#include "Color.h"
#include "DataStruct.h"

class UIButton : public UIElement {
public:
    UIButton(const SDL_Rect& rect, SDL_Texture* texture, const std::function<void()>& onClick);
    virtual ~UIButton() override; // declare

    void update(float deltaTime) override;
    void render(UIRenderer* renderer) override;
    bool handleInput(const SDL_Event& event) override;

private:
    SDL_Rect bounds;
    SDL_Texture* texture;
    std::function<void()> onClick;

    bool hovered = false;
    bool pressed = false;
};