#include "button.h"

bool GuiButton(UIButton button) {
    bool isClicked = false;
    Vector2 mousePos = GetMousePosition();
    
    bool isHovered = CheckCollisionPointRec(mousePos, button.bounds);

    if (isHovered && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        isClicked = true;
    }

    Color bgColor = isHovered ? button.hoverColor : button.baseColor;
    Color borderColor = isHovered ? WHITE : DARKGRAY;

    DrawRectangleRec(button.bounds, bgColor);
    DrawRectangleLinesEx(button.bounds, 2.0f, borderColor);

    int fontSize = 20;
    int textWidth = MeasureText(button.text, fontSize);
    int textX = button.bounds.x + (button.bounds.width / 2) - (textWidth / 2);
    int textY = button.bounds.y + (button.bounds.height / 2) - (fontSize / 2);

    DrawText(button.text, textX, textY, fontSize, button.textColor);

    return isClicked;
}