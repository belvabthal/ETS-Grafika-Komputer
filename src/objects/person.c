#include "person.h"
#include "coords.h"
#include "src/algo/bresenham.h"
#include "src/algo/midcircle.h" // Gunakan algoritma lingkaran dosen
#include <stdio.h>

void DrawComplexPerson(float cx, float cy, float scale, bool isOutlineMode) {
    float headR = 0.6f * scale;
    float headY = cy + 3.0f * scale;
    float bodyY_top = headY - headR;
    float bodyY_bot = cy + 1.2f * scale;
    float legY_bot = cy;
    float span = 0.8f * scale; // rentang kaki/tangan

    if (isOutlineMode) {
        Color outCol = LIGHTGRAY;
        Midcircle(CS_X(cx), CS_Y(headY), (int)(headR * G_TickStep), outCol); // Kepala
        BresenhamLine(CS_X(cx), CS_Y(bodyY_top), CS_X(cx), CS_Y(bodyY_bot), outCol); // Badan
        BresenhamLine(CS_X(cx), CS_Y(bodyY_bot), CS_X(cx - span), CS_Y(legY_bot), outCol); // Kaki Kiri
        BresenhamLine(CS_X(cx), CS_Y(bodyY_bot), CS_X(cx + span), CS_Y(legY_bot), outCol); // Kaki Kanan
        BresenhamLine(CS_X(cx - span), CS_Y(bodyY_top - 0.2f), CS_X(cx + span), CS_Y(bodyY_top - 0.2f), outCol); // Tangan
        
        char txt[30]; snprintf(txt, 30, "Head(%.1f, %.1f)", cx, headY);
        DrawText(txt, CS_X(cx)-20, CS_Y(headY + headR)-15, 10, YELLOW);
    } else {
        // Mode Penuh: Gunakan garis tebal dan lingkaran penuh
        MidcircleFilled(CS_X(cx), CS_Y(headY), (int)(headR * G_TickStep), BEIGE);
        Bres_ThickLine(CS_X(cx), CS_Y(bodyY_top), CS_X(cx), CS_Y(bodyY_bot), 12, DARKBLUE); // Baju
        Bres_ThickLine(CS_X(cx), CS_Y(bodyY_bot), CS_X(cx - span), CS_Y(legY_bot), 10, DARKGRAY); // Celana
        Bres_ThickLine(CS_X(cx), CS_Y(bodyY_bot), CS_X(cx + span), CS_Y(legY_bot), 10, DARKGRAY);
        Bres_ThickLine(CS_X(cx - span), CS_Y(bodyY_top - 0.2f), CS_X(cx + span), CS_Y(bodyY_top - 0.2f), 8, BEIGE); // Tangan
    }
}