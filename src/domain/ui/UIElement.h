#pragma once

#include <SDL.h>
#include <DataStruct.h>
#include "UIRenderer.h"
#include "UIInputHandler.h"
#include <string>

class UIElement
{
protected:
    Vector2 position;
    Vector2 size;
    bool visible = true;
    std::string elementID;
public:
    UIElement(const Vector2& pos, const Vector2& sz, const std::string& id = "")
    : position(pos), size(sz), elementID(id) {}

    virtual ~UIElement() = default;

    virtual void update(float deltaTime) = 0;
    virtual void render(UIRenderer* renderer) = 0;
    virtual bool handleInput(const SDL_Event& event) = 0;

    // Getters
    Vector2 getPosition() const { return position; }
    Vector2 getSize() const { return size; }
    bool isVisible() const { return visible; }
    std::string getID() const { return elementID; }

    // Setters
    void setPosition(const Vector2& pos) { position = pos; }
    void setSize(const Vector2& sz) { size = sz; }
    void setVisible(bool v) { visible = v; }
    void setID(const std::string& newID) { elementID = newID; }
};
