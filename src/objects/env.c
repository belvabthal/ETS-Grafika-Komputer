#include "env.h"
#include "coords.h"
#include "src/algo/bresenham.h" 
#include "src/algo/midcircle.h" // WAJIB DITAMBAHKAN untuk algoritma daun dan api
#include <stdio.h>

// Makro pemetaan presisi
#define MAP_X(x) (G_OriginX + (int)((x) * G_TickStep))
#define MAP_Y(y) (G_OriginY - (int)((y) * G_TickStep))

// Helper untuk mewarnai balok menggunakan Bresenham
static void FillRect(int xL, int xR, int yT, int yB, Color c) {
    for (int y = yT; y <= yB; y++) BresenhamLine(xL, y, xR, y, c);
}

// =========================================================================
// FUNGSI MATEMATIKA BARU: Teardrop (Gabungan Lingkaran & Segitiga Interpolasi)
// =========================================================================
static void DrawFlameTeardrop(int apexX, int apexY, int baseX, int baseY, int r, Color col, bool isOutline) {
    if (isOutline) {
        // Mode Blueprint: Gambar lingkaran alas dan 2 sisi miring segitiga
        Midcircle(baseX, baseY, r, col);
        BresenhamLine(baseX - r, baseY, apexX, apexY, col); // Sisi kiri
        BresenhamLine(baseX + r, baseY, apexX, apexY, col); // Sisi kanan
    } else {
        // Mode Penuh: Mewarnai dengan Scanline
        // 1. Gambar alas membulat (Lingkaran Penuh)
        MidcircleFilled(baseX, baseY, r, col);
        
        // 2. Gambar ujung meruncing (Interpolasi Segitiga dari Puncak ke Diameter Lingkaran)
        int dy = baseY - apexY; // baseY di layar nilainya lebih besar dari apexY (karena Y terbalik)
        if (dy > 0) {
            for (int y = apexY; y <= baseY; y++) {
                float t = (float)(y - apexY) / dy;
                // Batas alas segitiga adalah lebar diameter lingkaran (baseX - r) hingga (baseX + r)
                int xl = apexX + (int)(t * ((baseX - r) - apexX));
                int xr = apexX + (int)(t * ((baseX + r) - apexX));
                BresenhamLine(xl, y, xr, y, col);
            }
        }
    }
}

void DrawComplexTree(float cx, float cy, float scale, bool isOutlineMode) {
    float s = scale;

    // === 1. ARSITEKTUR KOORDINAT BATANG (TRUNK) ===
    int TRUNK_L = MAP_X(cx - 0.3f * s);
    int TRUNK_R = MAP_X(cx + 0.3f * s);
    int TRUNK_B = MAP_Y(cy - 1.0f * s); // Pangkal di tanah
    int TRUNK_T = MAP_Y(cy + 1.5f * s); // Ketinggian batang

    // === 2. ARSITEKTUR KANOPI DAUN (SEED OF LIFE) ===
    int C_X = MAP_X(cx);
    int C_Y = MAP_Y(cy + 2.2f * s); 
    int R = (int)(0.9f * s * G_TickStep); 

    int dx = (int)(R * 0.866025f); 
    int dy = (int)(R * 0.5f);      

    Color trunkCol = DARKBROWN;
    Color leafDark = DARKGREEN;   
    Color leafMid  = GREEN;       
    Color leafLight= LIME;        
    Color lineCol  = RAYWHITE;

    if (isOutlineMode) {
        BresenhamLine(TRUNK_L, TRUNK_B, TRUNK_R, TRUNK_B, lineCol);
        BresenhamLine(TRUNK_R, TRUNK_B, TRUNK_R, TRUNK_T, lineCol);
        BresenhamLine(TRUNK_R, TRUNK_T, TRUNK_L, TRUNK_T, lineCol);
        BresenhamLine(TRUNK_L, TRUNK_T, TRUNK_L, TRUNK_B, lineCol);

        Midcircle(C_X, C_Y, R, lineCol);             
        Midcircle(C_X, C_Y - R, R, lineCol);         
        Midcircle(C_X, C_Y + R, R, lineCol);         
        Midcircle(C_X + dx, C_Y - dy, R, lineCol);   
        Midcircle(C_X + dx, C_Y + dy, R, lineCol);   
        Midcircle(C_X - dx, C_Y - dy, R, lineCol);   
        Midcircle(C_X - dx, C_Y + dy, R, lineCol);   

        char txt[30]; snprintf(txt, 30, "Radius Daun: %dpx", R);
        DrawText(txt, C_X - 40, C_Y - R - 20, 10, YELLOW);
    } else {
        FillRect(TRUNK_L, TRUNK_R, TRUNK_T, TRUNK_B, trunkCol);

        MidcircleFilled(C_X, C_Y + R, R, leafDark);
        MidcircleFilled(C_X - dx, C_Y + dy, R, leafDark);
        MidcircleFilled(C_X + dx, C_Y + dy, R, leafDark);

        MidcircleFilled(C_X - dx, C_Y - dy, R, leafMid);
        MidcircleFilled(C_X + dx, C_Y - dy, R, leafMid);
        MidcircleFilled(C_X, C_Y, R, leafMid); 

        MidcircleFilled(C_X, C_Y - R, R, leafLight);
    }
}

void DrawComplexCampfire(float cx, float cy, float scale, bool isOutlineMode) {
    float s = scale;

    // 1. KAYU BAKAR (Log Geometri)
    float logL_x0 = cx - 1.5f * s, logL_y0 = cy + 0.5f * s;
    float logL_x1 = cx + 1.5f * s, logL_y1 = cy - 0.5f * s;
    
    float logR_x0 = cx - 1.5f * s, logR_y0 = cy - 0.5f * s;
    float logR_x1 = cx + 1.5f * s, logR_y1 = cy + 0.5f * s;

    // 2. TATA LETAK 4 API (Teardrop Geometrics)
    // Api Utama (Tengah, Besar)
    int mainBaseX = MAP_X(cx);
    int mainBaseY = MAP_Y(cy);
    int mainApexX = MAP_X(cx);
    int mainApexY = MAP_Y(cy + 2.5f * s);
    int mainR = (int)(0.8f * s * G_TickStep);

    // Api Kiri (Condong Kiri, Sedang)
    int leftBaseX = MAP_X(cx - 0.6f * s);
    int leftBaseY = MAP_Y(cy - 0.2f * s);
    int leftApexX = MAP_X(cx - 1.2f * s);  // Puncak digeser ke kiri
    int leftApexY = MAP_Y(cy + 1.5f * s);
    int leftR = (int)(0.5f * s * G_TickStep);

    // Api Kanan (Condong Kanan, Sedang)
    int rightBaseX = MAP_X(cx + 0.6f * s);
    int rightBaseY = MAP_Y(cy - 0.2f * s);
    int rightApexX = MAP_X(cx + 1.2f * s); // Puncak digeser ke kanan
    int rightApexY = MAP_Y(cy + 1.5f * s);
    int rightR = (int)(0.5f * s * G_TickStep);

    // Api Dalam (Tengah, Kecil)
    int innerBaseX = MAP_X(cx);
    int innerBaseY = MAP_Y(cy);
    int innerApexX = MAP_X(cx);
    int innerApexY = MAP_Y(cy + 1.2f * s);
    int innerR = (int)(0.4f * s * G_TickStep);

    if (isOutlineMode) {
        Color outCol = LIGHTGRAY;
        // Kayu menyilang
        BresenhamLine(MAP_X(logL_x0), MAP_Y(logL_y0), MAP_X(logL_x1), MAP_Y(logL_y1), DARKBROWN);
        BresenhamLine(MAP_X(logR_x0), MAP_Y(logR_y0), MAP_X(logR_x1), MAP_Y(logR_y1), DARKBROWN);
        
        // Render Blueprint Api
        DrawFlameTeardrop(mainApexX, mainApexY, mainBaseX, mainBaseY, mainR, outCol, true);
        DrawFlameTeardrop(leftApexX, leftApexY, leftBaseX, leftBaseY, leftR, outCol, true);
        DrawFlameTeardrop(rightApexX, rightApexY, rightBaseX, rightBaseY, rightR, outCol, true);
        DrawFlameTeardrop(innerApexX, innerApexY, innerBaseX, innerBaseY, innerR, outCol, true);
        
        char txt[30]; snprintf(txt, 30, "Api(%.1f, %.1f)", cx, cy + 2.5f*scale);
        DrawText(txt, mainApexX - 30, mainApexY - 15, 10, ORANGE);
    } else {
        // Mode Solid Kayu
        Bres_ThickLine(MAP_X(logL_x0), MAP_Y(logL_y0), MAP_X(logL_x1), MAP_Y(logL_y1), 10, DARKBROWN);
        Bres_ThickLine(MAP_X(logR_x0), MAP_Y(logR_y0), MAP_X(logR_x1), MAP_Y(logR_y1), 10, DARKBROWN);
        
        // Mode Solid Api (Urutan render dari belakang ke depan menentukan Z-Index visual)
        DrawFlameTeardrop(mainApexX, mainApexY, mainBaseX, mainBaseY, mainR, RED, false);
        DrawFlameTeardrop(leftApexX, leftApexY, leftBaseX, leftBaseY, leftR, ORANGE, false);
        DrawFlameTeardrop(rightApexX, rightApexY, rightBaseX, rightBaseY, rightR, ORANGE, false);
        DrawFlameTeardrop(innerApexX, innerApexY, innerBaseX, innerBaseY, innerR, YELLOW, false);
    }
}