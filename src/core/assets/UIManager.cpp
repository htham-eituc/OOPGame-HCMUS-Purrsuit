#include "UIManager.h"

UIManager::UIManager(UIRenderer* renderer)
    : renderer(renderer) {}

UIManager::~UIManager() {}

void UIManager::addElement(std::shared_ptr<UIElement> element) {
    elements.push_back(element);
    inputHandler.registerElement(element);
}

void UIManager::clear() {
    elements.clear();
    inputHandler.clearElements();
}

void UIManager::update(float deltaTime) {
    for (auto& element : elements) {
        if (element->isVisible()) {
            element->update(deltaTime);
        }
    }
}

void UIManager::render() {
    for (auto& element : elements) {
        if (element->isVisible()) {
            element->render(renderer);
        }
    }
}

bool UIManager::handleEvent(const SDL_Event& event) {
    return inputHandler.handleEvent(event);
}
