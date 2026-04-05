#include "car.h"
#include "coords.h"
#include "src/algo/bresenham.h" 
#include "src/algo/midcircle.h" 
#include <math.h> 
#include <stdio.h>

#ifndef PI
#define PI 3.14159265358979323846f
#endif

static void OutlineRect(int xL, int xR, int yT, int yB, Color c) {
    BresenhamLine(xL, yT, xR, yT, c); 
    BresenhamLine(xR, yT, xR, yB, c); 
    BresenhamLine(xR, yB, xL, yB, c); 
    BresenhamLine(xL, yB, xL, yT, c); 
}

static void FillRect(int xL, int xR, int yT, int yB, Color c) {
    for (int y = yT; y <= yB; y++) {
        BresenhamLine(xL, y, xR, y, c);
    }
}

static void DrawWheelSpokes(int wx, int wy, int r, float angle, int thickness, Color c) {
    for (int i = 0; i < 3; i++) {
        float theta = angle + (i * PI / 3.0f);
        int dx = (int)(cosf(theta) * r);
        int dy = (int)(sinf(theta) * r);
        
        if (thickness > 1) {
            Bres_ThickLine(wx - dx, wy - dy, wx + dx, wy + dy, thickness, c);
        } else {
            BresenhamLine(wx - dx, wy - dy, wx + dx, wy + dy, c);
        }
    }
}

void DrawComplexCar(float cx, float cy, float scale, bool isOutlineMode, float wheelAngle) {
    float s = scale;

    // === ARSITEKTUR MOBIL DI KARTESIAN ===
    int X_LAMP_FL = MAP_X(cx - 4.5f * s); 
    int X_FRONT   = MAP_X(cx - 4.0f * s); 
    int X_CABIN_L = MAP_X(cx - 1.0f * s); 
    int X_ROOF_L  = MAP_X(cx + 0.0f * s); 
    int X_CABIN_R = MAP_X(cx + 2.0f * s); 
    int X_BACK    = MAP_X(cx + 5.0f * s); 
    int X_LAMP_BR = MAP_X(cx + 5.5f * s); 

    int Y_TOP = MAP_Y(cy + 3.0f * s); 
    int Y_MID = MAP_Y(cy + 1.0f * s); 
    int Y_BOT = MAP_Y(cy - 1.0f * s); 

    int Y_LAMP_T = MAP_Y(cy + 0.5f * s);
    int Y_LAMP_B = MAP_Y(cy - 0.5f * s);

    int CG_X0 = MAP_X(cx + 2.5f * s);
    int CG_X1 = MAP_X(cx + 4.5f * s);
    int CG_Y_BASE = Y_MID;
    int CG_Y_TOP  = MAP_Y(cy + 1.6f * s);
    int CG_Y_CEN  = MAP_Y(cy + 1.3f * s);
    int CG_R      = (int)(0.3f * s * G_TickStep);

    int WIN_X_LB = MAP_X(cx - 0.7f * s);
    int WIN_X_LT = MAP_X(cx + 0.2f * s);
    int WIN_X_R  = MAP_X(cx + 1.7f * s);
    int WIN_Y_T  = MAP_Y(cy + 2.7f * s);
    int WIN_Y_B  = MAP_Y(cy + 1.3f * s);

    int W1_X = MAP_X(cx - 2.5f * s);
    int W2_X = MAP_X(cx + 3.5f * s);
    int W_Y  = MAP_Y(cy - 1.0f * s); 
    int W_R  = (int)(1.0f * s * G_TickStep);

    Color carCol  = (Color){45, 120, 190, 255}; 
    Color lineCol = RAYWHITE;

    if (isOutlineMode) {
        BresenhamLine(X_FRONT, Y_BOT, X_BACK, Y_BOT, lineCol); 
        BresenhamLine(X_FRONT, Y_BOT, X_FRONT, Y_MID, lineCol); 
        BresenhamLine(X_FRONT, Y_MID, X_CABIN_L, Y_MID, lineCol); 
        BresenhamLine(X_CABIN_L, Y_MID, X_ROOF_L, Y_TOP, lineCol); 
        BresenhamLine(X_ROOF_L, Y_TOP, X_CABIN_R, Y_TOP, lineCol); 
        BresenhamLine(X_CABIN_R, Y_TOP, X_CABIN_R, Y_MID, lineCol); 
        BresenhamLine(X_CABIN_R, Y_MID, X_BACK, Y_MID, lineCol); 
        BresenhamLine(X_BACK, Y_MID, X_BACK, Y_BOT, lineCol); 
        BresenhamLine(X_CABIN_L, Y_BOT, X_CABIN_L, Y_MID, lineCol);

        OutlineRect(X_LAMP_FL, X_FRONT, Y_LAMP_T, Y_LAMP_B, lineCol);
        OutlineRect(X_BACK, X_LAMP_BR, Y_LAMP_T, Y_LAMP_B, lineCol);

        BresenhamLine(WIN_X_LB, WIN_Y_B, WIN_X_R, WIN_Y_B, lineCol);
        BresenhamLine(WIN_X_R, WIN_Y_B, WIN_X_R, WIN_Y_T, lineCol);
        BresenhamLine(WIN_X_R, WIN_Y_T, WIN_X_LT, WIN_Y_T, lineCol);
        BresenhamLine(WIN_X_LT, WIN_Y_T, WIN_X_LB, WIN_Y_B, lineCol);

        BresenhamLine(CG_X0, CG_Y_TOP, CG_X1, CG_Y_TOP, lineCol);
        BresenhamLine(CG_X0, CG_Y_BASE, CG_X1, CG_Y_BASE, lineCol);
        Midcircle(CG_X0, CG_Y_CEN, CG_R, lineCol);
        Midcircle(CG_X1, CG_Y_CEN, CG_R, lineCol);

        // Roda & Rotasi
        Midcircle(W1_X, W_Y, W_R, lineCol);
        Midcircle(W1_X, W_Y, (int)(W_R * 0.45f), lineCol);
        Midcircle(W2_X, W_Y, W_R, lineCol);
        Midcircle(W2_X, W_Y, (int)(W_R * 0.45f), lineCol);
        DrawWheelSpokes(W1_X, W_Y, W_R, wheelAngle, 1, lineCol);
        Midcircle(W2_X, W_Y, (int)(W_R * 0.45f), lineCol);
        DrawWheelSpokes(W2_X, W_Y, W_R, wheelAngle, 1, lineCol);

    } else {
        FillRect(X_LAMP_FL, X_FRONT, Y_LAMP_T, Y_LAMP_B, YELLOW);
        FillRect(X_BACK, X_LAMP_BR, Y_LAMP_T, Y_LAMP_B, RED);
        FillRect(X_FRONT, X_BACK, Y_MID, Y_BOT, carCol);

        int dy_cabin = Y_MID - Y_TOP; 
        if (dy_cabin > 0) {
            for (int y = Y_TOP; y <= Y_MID; y++) {
                float t = (float)(y - Y_TOP) / dy_cabin;
                int current_xL = X_ROOF_L + (int)(t * (X_CABIN_L - X_ROOF_L));
                BresenhamLine(current_xL, y, X_CABIN_R, y, carCol);
            }
        }
        Bres_ThickLine(X_CABIN_L, Y_BOT, X_CABIN_L, Y_MID, 2, DARKBLUE);

        int dy_win = WIN_Y_B - WIN_Y_T;
        if (dy_win > 0) {
            for (int y = WIN_Y_T; y <= WIN_Y_B; y++) {
                float t = (float)(y - WIN_Y_T) / dy_win;
                int current_xL = WIN_X_LT + (int)(t * (WIN_X_LB - WIN_X_LT));
                BresenhamLine(current_xL, y, WIN_X_R, y, SKYBLUE);
            }
        }

        FillRect(CG_X0, CG_X1, CG_Y_TOP, CG_Y_BASE, DARKBROWN); 
        MidcircleFilled(CG_X0, CG_Y_CEN, CG_R, (Color){100, 60, 40, 255}); 
        MidcircleFilled(CG_X1, CG_Y_CEN, CG_R, (Color){100, 60, 40, 255}); 

        // Roda & Rotasi (Solid)
        MidcircleFilled(W1_X, W_Y, W_R, BLACK);
        MidcircleFilled(W1_X, W_Y, (int)(W_R * 0.45f), LIGHTGRAY);
        DrawWheelSpokes(W1_X, W_Y, W_R - 2, wheelAngle, 4, DARKGRAY);

        MidcircleFilled(W2_X, W_Y, W_R, BLACK);
        MidcircleFilled(W2_X, W_Y, (int)(W_R * 0.45f), LIGHTGRAY);
        DrawWheelSpokes(W2_X, W_Y, W_R - 2, wheelAngle, 4, DARKGRAY);
    }
}