#pragma once

#include "UIElement.h"
#include <SDL_ttf.h>
#include <string>
#include <Color.h>

class UILabel : public UIElement
{
private:
    std::string text;
    Color color;
    TTF_Font* font;

    bool outlineEnabled = false;
    Color outlineColor;
public:
    UILabel(const Vector2& pos, const Vector2& sz, const std::string& txt, const Color& clr, const Color& outlineColor,TTF_Font* fnt, const std::string& id = "")
        : UIElement(pos, sz, id), text(txt), color(clr), outlineColor(outlineColor), font(fnt) {}

    void setText(const std::string& newText) { text = newText; }
    void setColor(const Color& newColor) { color = newColor; }
    void setFont(TTF_Font* newFont) { font = newFont; }

    const std::string& getText() const { return text; }
    const Color& getColor() const { return color; }
    TTF_Font* getFont() const { return font; }

    void enableOutline(Color color) {
        outlineEnabled = true;
        outlineColor = color;
    }

    void disableOutline() {
        outlineEnabled = false;
    }

    void update(float deltaTime) override {} // Static label

    void render(UIRenderer* renderer) override;

    bool handleInput(const SDL_Event& event) override { return false; }
};
