#include "car.h"
#include "coords.h"
#include "src/algo/bresenham.h" 
#include "src/algo/midcircle.h" 
#include <stdio.h>

// === SOLUSI PRESISI PIKSEL ===
// Kita buat macro pemetaan sendiri yang mengalikan desimal (float) 
// DENGAN skala terlebih dahulu, baru diubah ke integer (piksel layar).
// Ini mencegah pemotongan desimal secara prematur!
#define MAP_X(x) (G_OriginX + (int)((x) * G_TickStep))
#define MAP_Y(y) (G_OriginY - (int)((y) * G_TickStep))

static void OutlineRect(int xL, int xR, int yT, int yB, Color c) {
    BresenhamLine(xL, yT, xR, yT, c); // Atas
    BresenhamLine(xR, yT, xR, yB, c); // Kanan
    BresenhamLine(xR, yB, xL, yB, c); // Bawah
    BresenhamLine(xL, yB, xL, yT, c); // Kiri
}

static void FillRect(int xL, int xR, int yT, int yB, Color c) {
    for (int y = yT; y <= yB; y++) BresenhamLine(xL, y, xR, y, c);
}

void DrawComplexCar(float cx, float cy, float scale, bool isOutlineMode) {
    float s = scale;

    // === 1. ARSITEKTUR KARTESIAN PRESISI TINGGI (SKETSA BELVA) ===
    
    // Sumbu X Utama (Proporsi Simetri Sempurna 3-3-3 Blok)
    int X_LAMP_FL = MAP_X(cx - 4.5f * s); // Kiri Lampu Depan
    int X_FRONT   = MAP_X(cx - 4.0f * s); // Kiri Kap Mesin
    int X_CABIN_L = MAP_X(cx - 1.0f * s); // Batas Kap & Kabin
    int X_ROOF_L  = MAP_X(cx + 0.0f * s); // Ujung Atap Kaca Miring
    int X_CABIN_R = MAP_X(cx + 2.0f * s); // Batas Kabin & Bak
    int X_BACK    = MAP_X(cx + 5.0f * s); // Kanan Bak Belakang
    int X_LAMP_BR = MAP_X(cx + 5.5f * s); // Kanan Lampu Belakang

    // Sumbu Y Utama
    int Y_TOP = MAP_Y(cy + 3.0f * s); // Atap Kabin Tertinggi
    int Y_MID = MAP_Y(cy + 1.0f * s); // Tinggi Kap & Bak
    int Y_BOT = MAP_Y(cy - 1.0f * s); // Garis Bawah Bodi (Pusat Roda)

    // Koordinat Lampu (Terpotong di tengah X_FRONT dan X_BACK)
    int Y_LAMP_T = MAP_Y(cy + 0.5f * s);
    int Y_LAMP_B = MAP_Y(cy - 0.5f * s);

    // Koordinat Kargo Silinder (X=2.5 hingga 4.5, di atas Bak Y=1)
    int CG_X0 = MAP_X(cx + 2.5f * s);
    int CG_X1 = MAP_X(cx + 4.5f * s);
    int CG_Y_BASE = Y_MID;
    int CG_Y_TOP  = MAP_Y(cy + 1.6f * s);
    int CG_Y_CEN  = MAP_Y(cy + 1.3f * s);
    int CG_R      = (int)(0.3f * s * G_TickStep);

    // Koordinat Roda (Pusat roda tepat di Y = -1, memotong bodi)
    int W1_X = MAP_X(cx - 2.5f * s);
    int W2_X = MAP_X(cx + 3.5f * s);
    int W_Y  = Y_BOT; 
    int W_R  = (int)(1.0f * s * G_TickStep);

    // Jendela (Trapesium Kecil)
    int WIN_X_LB = MAP_X(cx - 0.7f * s);
    int WIN_X_LT = MAP_X(cx + 0.2f * s);
    int WIN_X_R  = MAP_X(cx + 1.7f * s);
    int WIN_Y_T  = MAP_Y(cy + 2.7f * s);
    int WIN_Y_B  = MAP_Y(cy + 1.3f * s);

    // Palet Warna
    Color carCol  = (Color){45, 120, 190, 255}; 
    Color lineCol = RAYWHITE;

    // === 2. MESIN RENDER VISUAL ===

    if (isOutlineMode) {
        // --- MODE BLUEPRINT ---

        // Bodi Utama
        BresenhamLine(X_FRONT, Y_BOT, X_BACK, Y_BOT, lineCol); // Garis bawah
        BresenhamLine(X_FRONT, Y_BOT, X_FRONT, Y_MID, lineCol); // Muka depan
        BresenhamLine(X_FRONT, Y_MID, X_CABIN_L, Y_MID, lineCol); // Kap mesin
        BresenhamLine(X_CABIN_L, Y_MID, X_ROOF_L, Y_TOP, lineCol); // Kaca miring
        BresenhamLine(X_ROOF_L, Y_TOP, X_CABIN_R, Y_TOP, lineCol); // Atap
        BresenhamLine(X_CABIN_R, Y_TOP, X_CABIN_R, Y_MID, lineCol); // Punggung kabin
        BresenhamLine(X_CABIN_R, Y_MID, X_BACK, Y_MID, lineCol); // Bak atas
        BresenhamLine(X_BACK, Y_MID, X_BACK, Y_BOT, lineCol); // Pintu bak belakang
        
        // Garis Pembatas Pintu
        BresenhamLine(X_CABIN_L, Y_BOT, X_CABIN_L, Y_MID, lineCol);

        // Lampu (Menonjol setengah)
        OutlineRect(X_LAMP_FL, X_FRONT, Y_LAMP_T, Y_LAMP_B, lineCol);
        OutlineRect(X_BACK, X_LAMP_BR, Y_LAMP_T, Y_LAMP_B, lineCol);

        // Jendela
        BresenhamLine(WIN_X_LB, WIN_Y_B, WIN_X_R, WIN_Y_B, lineCol);
        BresenhamLine(WIN_X_R, WIN_Y_B, WIN_X_R, WIN_Y_T, lineCol);
        BresenhamLine(WIN_X_R, WIN_Y_T, WIN_X_LT, WIN_Y_T, lineCol);
        BresenhamLine(WIN_X_LT, WIN_Y_T, WIN_X_LB, WIN_Y_B, lineCol);

        // Kargo Silinder
        BresenhamLine(CG_X0, CG_Y_TOP, CG_X1, CG_Y_TOP, lineCol);
        BresenhamLine(CG_X0, CG_Y_BASE, CG_X1, CG_Y_BASE, lineCol);
        Midcircle(CG_X0, CG_Y_CEN, CG_R, lineCol);
        Midcircle(CG_X1, CG_Y_CEN, CG_R, lineCol);

        // Roda (Memotong garis bawah)
        Midcircle(W1_X, W_Y, W_R, lineCol);
        Midcircle(W2_X, W_Y, W_R, lineCol);
        BresenhamLine(W1_X - W_R, W_Y, W1_X + W_R, W_Y, lineCol); // Palang
        BresenhamLine(W1_X, W_Y - W_R, W1_X, W_Y + W_R, lineCol);
        BresenhamLine(W2_X - W_R, W_Y, W2_X + W_R, W_Y, lineCol);
        BresenhamLine(W2_X, W_Y - W_R, W2_X, W_Y + W_R, lineCol);

        char txt[30]; snprintf(txt, 30, "Pusat Roda (y=-1)");
        DrawText(txt, W1_X - 40, W_Y + 20, 10, YELLOW);

    } else {
        // --- MODE PENUH (WARNA SOLID) ---

        // Lampu
        FillRect(X_LAMP_FL, X_FRONT, Y_LAMP_T, Y_LAMP_B, YELLOW);
        FillRect(X_BACK, X_LAMP_BR, Y_LAMP_T, Y_LAMP_B, RED);

        // Kap Depan & Bak Belakang
        FillRect(X_FRONT, X_BACK, Y_MID, Y_BOT, carCol);

        // Kabin (Trapesium)
        int dy_cabin = Y_MID - Y_TOP; 
        if (dy_cabin > 0) {
            for (int y = Y_TOP; y <= Y_MID; y++) {
                float t = (float)(y - Y_TOP) / dy_cabin;
                int current_xL = X_ROOF_L + (int)(t * (X_CABIN_L - X_ROOF_L));
                BresenhamLine(current_xL, y, X_CABIN_R, y, carCol);
            }
        }

        // Garis Pembatas Pintu
        Bres_ThickLine(X_CABIN_L, Y_BOT, X_CABIN_L, Y_MID, 2, DARKBLUE);

        // Jendela
        int dy_win = WIN_Y_B - WIN_Y_T;
        if (dy_win > 0) {
            for (int y = WIN_Y_T; y <= WIN_Y_B; y++) {
                float t = (float)(y - WIN_Y_T) / dy_win;
                int current_xL = WIN_X_LT + (int)(t * (WIN_X_LB - WIN_X_LT));
                BresenhamLine(current_xL, y, WIN_X_R, y, SKYBLUE);
            }
        }

        // Kargo Silinder
        FillRect(CG_X0, CG_X1, CG_Y_TOP, CG_Y_BASE, DARKBROWN); 
        MidcircleFilled(CG_X0, CG_Y_CEN, CG_R, (Color){100, 60, 40, 255}); 
        MidcircleFilled(CG_X1, CG_Y_CEN, CG_R, (Color){100, 60, 40, 255}); 

        // Roda (Lapisan paling depan menutupi sasis)
        MidcircleFilled(W1_X, W_Y, W_R, BLACK);
        MidcircleFilled(W1_X, W_Y, (int)(W_R * 0.45f), LIGHTGRAY);
        Bres_ThickLine(W1_X - W_R + 2, W_Y, W1_X + W_R - 2, W_Y, 4, DARKGRAY);
        Bres_ThickLine(W1_X, W_Y - W_R + 2, W1_X, W_Y + W_R - 2, 4, DARKGRAY);

        MidcircleFilled(W2_X, W_Y, W_R, BLACK);
        MidcircleFilled(W2_X, W_Y, (int)(W_R * 0.45f), LIGHTGRAY);
        Bres_ThickLine(W2_X - W_R + 2, W_Y, W2_X + W_R - 2, W_Y, 4, DARKGRAY);
        Bres_ThickLine(W2_X, W_Y - W_R + 2, W2_X, W_Y + W_R - 2, 4, DARKGRAY);
    }
}