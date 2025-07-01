#include "UIInputHandler.h"
#include "UIElement.h"
#include <algorithm>

void UIInputHandler::registerElement(std::shared_ptr<UIElement> element) {
    elements.push_back(element);
}

void UIInputHandler::unregisterElement(std::shared_ptr<UIElement> element) {
    elements.erase(
        std::remove(elements.begin(), elements.end(), element),
        elements.end()
    );
}

void UIInputHandler::clearElements() {
    elements.clear();
}

bool UIInputHandler::handleEvent(const SDL_Event& event) {
    for (auto& element : elements) {
        if (element->isVisible() && element->handleInput(event)) {
            return true; // Event was consumed
        }
    }
    return false; // Event was not handled by any UI element
}