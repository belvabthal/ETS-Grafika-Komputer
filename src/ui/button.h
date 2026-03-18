#ifndef BUTTON_H
#define BUTTON_H

#include <raylib.h>
#include <stdbool.h>

typedef struct {
    Rectangle bounds;    
    const char* text;    
    Color baseColor;     
    Color hoverColor;    
    Color textColor;     
} UIButton;

bool GuiButton(UIButton button);

#endif