#pragma once

#include <vector>
#include <memory>
#include <SDL.h>

class UIElement;

class UIInputHandler 
{
public:
    void registerElement(std::shared_ptr<UIElement> element);
    void clearElements();
    bool handleEvent(const SDL_Event& event);

private:
    std::vector<std::shared_ptr<UIElement>> elements;
};