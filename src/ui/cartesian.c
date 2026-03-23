#include "src/ui/cartesian.h"
#include "coords.h"
#include "screen_type.h"
#include <stdio.h>

void DrawCartesianGrid(void (*lineFunc)(int,int,int,int,Color)) {
    for (int i = -7; i <= 7; i++) {
        lineFunc(CS_X(i), CS_Y(-8), CS_X(i), CS_Y(8), (Color){30, 50, 35, 255});
        lineFunc(CS_X(-8), CS_Y(i), CS_X(8), CS_Y(i), (Color){30, 50, 35, 255});
    }

    lineFunc(CS_X(-8), CS_Y(0), CS_X(8), CS_Y(0), (Color){160, 170, 160, 255});
    lineFunc(CS_X(0), CS_Y(-8), CS_X(0), CS_Y(8), (Color){160, 170, 160, 255});

    lineFunc(CS_X(8), CS_Y(0), CS_X(8)-10, CS_Y(0)-6, (Color){160, 170, 160, 255});
    lineFunc(CS_X(8), CS_Y(0), CS_X(8)-10, CS_Y(0)+6, (Color){160, 170, 160, 255});
    lineFunc(CS_X(0), CS_Y(8), CS_X(0)-6, CS_Y(8)+10, (Color){160, 170, 160, 255});
    lineFunc(CS_X(0), CS_Y(8), CS_X(0)+6, CS_Y(8)+10, (Color){160, 170, 160, 255});

    DrawText("X",  CS_X(8)+8,  CS_Y(0)-9, 18, (Color){160, 170, 160, 255});
    DrawText("Y",  CS_X(0)+8,  CS_Y(8)-22, 18, (Color){160, 170, 160, 255});
    DrawText("(0,0)", CS_X(0)+5, CS_Y(0)+5, 11, (Color){120,130,120,255});

    for (int i = -7; i <= 7; i++) {
        if (i == 0) continue;
        lineFunc(CS_X(i), CS_Y(0)-4, CS_X(i), CS_Y(0)+4, (Color){160, 170, 160, 255});
        lineFunc(CS_X(0)-4, CS_Y(i), CS_X(0)+4, CS_Y(i), (Color){160, 170, 160, 255});

        char lbl[8];
        sprintf(lbl, "%d", i);
        DrawText(lbl, CS_X(i)-5, CS_Y(0)+8, 11, (Color){130,140,130,255});
        sprintf(lbl, "%d", i);
        DrawText(lbl, CS_X(0)+6, CS_Y(i)-6, 11, (Color){130,140,130,255});
    }
}