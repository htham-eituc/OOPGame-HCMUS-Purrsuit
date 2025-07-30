#include "UILabel.h"
#include "UIRenderer.h"

void UILabel::render(UIRenderer* renderer)
{
    if (!visible || text.empty() || !font) return;

    if (outlineEnabled) {
        renderer->drawTextWithOutline(text, font, color, outlineColor, position);
    } else {
        renderer->drawText(text, font, color, position);
    }
}
