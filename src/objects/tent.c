#include "tent.h"
#include "coords.h"
#include "src/algo/bresenham.h" 
#include <stdio.h>

void DrawComplexTent(float cx, float cy, float scale, bool isOutlineMode) {
    // 1. DIMENSI DASAR
    float width = 6.0f * scale;  
    float height = 4.0f * scale; 
    float doorWidth = 2.0f * scale;
    float doorHeight = 2.5f * scale;

    // 2. PEMETAAN KOORDINAT (Titik x,y)
    // Titik Tenda Utama (Segitiga Luar)
    float x_t0 = cx - (width / 2); // Kiri bawah
    float y_t0 = cy;
    float x_t1 = cx + (width / 2); // Kanan bawah
    float y_t1 = cy;
    float x_t2 = cx;               // Puncak tenda
    float y_t2 = cy + height;

    // Titik Pintu Tenda (Segitiga Dalam)
    float x_d0 = cx - (doorWidth / 2);
    float y_d0 = cy;
    float x_d1 = cx + (doorWidth / 2);
    float y_d1 = cy;
    float x_d2 = cx;
    float y_d2 = cy + doorHeight;

    // Titik Tali & Pasak (Menambah detail presisi)
    float x_ropeLeft = cx - (width / 2) - (1.5f * scale);
    float y_ropeLeft = cy;
    float x_ropeRight = cx + (width / 2) + (1.5f * scale);
    float y_ropeRight = cy;

    if (isOutlineMode) {
        // --- MODE BLUEPRINT (NILAI MAKSIMAL UNTUK METODE WHITEBOARD) ---
        Color lineCol = RAYWHITE;
        
        // Outline Tenda Utama
        BresenhamLine(CS_X(x_t0), CS_Y(y_t0), CS_X(x_t1), CS_Y(y_t1), lineCol);
        BresenhamLine(CS_X(x_t1), CS_Y(y_t1), CS_X(x_t2), CS_Y(y_t2), lineCol);
        BresenhamLine(CS_X(x_t2), CS_Y(y_t2), CS_X(x_t0), CS_Y(y_t0), lineCol);

        // Outline Pintu
        BresenhamLine(CS_X(x_d0), CS_Y(y_d0), CS_X(x_d1), CS_Y(y_d1), lineCol);
        BresenhamLine(CS_X(x_d1), CS_Y(y_d1), CS_X(x_d2), CS_Y(y_d2), lineCol);
        BresenhamLine(CS_X(x_d2), CS_Y(y_d2), CS_X(x_d0), CS_Y(y_d0), lineCol);

        // Tali Tenda (Detail Ekstra)
        BresenhamLine(CS_X(x_t2), CS_Y(y_t2), CS_X(x_ropeLeft), CS_Y(y_ropeLeft), DARKGRAY);
        BresenhamLine(CS_X(x_t2), CS_Y(y_t2), CS_X(x_ropeRight), CS_Y(y_ropeRight), DARKGRAY);

        // Pasak Tenda (Garis Vertikal Kecil)
        BresenhamLine(CS_X(x_ropeLeft), CS_Y(y_ropeLeft), CS_X(x_ropeLeft), CS_Y(y_ropeLeft - 0.5f), RED);
        BresenhamLine(CS_X(x_ropeRight), CS_Y(y_ropeRight), CS_X(x_ropeRight), CS_Y(y_ropeRight - 0.5f), RED);

        // Cetak Koordinat Puncak
        char txt[30];
        snprintf(txt, 30, "Puncak (%.1f, %.1f)", cx, cy + height);
        DrawText(txt, CS_X(x_t2) - 30, CS_Y(y_t2) - 20, 10, YELLOW);

    } else {
        // --- MODE PENUH (SCANLINE BRESENHAM) ---
        // 1. Mewarnai Kanopi Utama (Oranye)
        int sy_top = CS_Y(y_t2);
        int sy_bottom = CS_Y(y_t0);
        int sx_top = CS_X(x_t2);
        int sx_left = CS_X(x_t0);
        int sx_right = CS_X(x_t1);

        int deltaY = sy_bottom - sy_top;
        if (deltaY > 0) {
            for (int y = sy_top; y <= sy_bottom; y++) {
                float t = (float)(y - sy_top) / (float)deltaY;
                int cX_left = sx_top + (int)(t * (sx_left - sx_top));
                int cX_right = sx_top + (int)(t * (sx_right - sx_top));
                BresenhamLine(cX_left, y, cX_right, y, ORANGE);
            }
        }

        // 2. Mewarnai Pintu Tenda (Coklat Gelap) ditimpa di atas kanopi
        int sdy_top = CS_Y(y_d2);
        int sdy_bottom = CS_Y(y_d0);
        int sdx_top = CS_X(x_d2);
        int sdx_left = CS_X(x_d0);
        int sdx_right = CS_X(x_d1);

        int d_deltaY = sdy_bottom - sdy_top;
        if (d_deltaY > 0) {
            for (int y = sdy_top; y <= sdy_bottom; y++) {
                float t = (float)(y - sdy_top) / (float)d_deltaY;
                int cX_left = sdx_top + (int)(t * (sdx_left - sdx_top));
                int cX_right = sdx_top + (int)(t * (sdx_right - sdx_top));
                BresenhamLine(cX_left, y, cX_right, y, DARKBROWN);
            }
        }
        
        // Render Tali (agar tetap terlihat di mode penuh)
        BresenhamLine(CS_X(x_t2), CS_Y(y_t2), CS_X(x_ropeLeft), CS_Y(y_ropeLeft), DARKGRAY);
        BresenhamLine(CS_X(x_t2), CS_Y(y_t2), CS_X(x_ropeRight), CS_Y(y_ropeRight), DARKGRAY);
    }
}