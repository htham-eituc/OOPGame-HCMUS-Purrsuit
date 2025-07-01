#pragma once

#include <vector>
#include <memory>
#include <SDL.h>
#include "UIElement.h"
#include "UIRenderer.h"
#include "UIInputHandler.h"

class UIManager
{
public:
    UIManager(UIRenderer* renderer);
    ~UIManager();

    void update(float deltaTime);
    void render();
    bool handleEvent(const SDL_Event& event);

    void addElement(std::shared_ptr<UIElement> element);
    void clear();

private:
    UIRenderer* renderer;
    UIInputHandler inputHandler;
    std::vector<std::shared_ptr<UIElement>> elements;
};