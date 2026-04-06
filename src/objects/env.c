#include "env.h"
#include "coords.h"
#include "src/algo/bresenham.h" 
#include "src/algo/midcircle.h" 
#include <math.h>
#include <stdio.h>

static void FillRect(int xL, int xR, int yT, int yB, Color c) {
    for (int y = yT; y <= yB; y++) BresenhamLine(xL, y, xR, y, c);
}

static void DrawFlameTeardrop(int apexX, int apexY, int baseX, int baseY, int r, Color col, bool isOutline) {
    if (isOutline) {
        Midcircle(baseX, baseY, r, col);
        BresenhamLine(baseX - r, baseY, apexX, apexY, col); // Sisi kiri
        BresenhamLine(baseX + r, baseY, apexX, apexY, col); // Sisi kanan
    } else {
        // Gambar alas membulat 
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

    // === ARSITEKTUR KOORDINAT BATANG (TRUNK) ===
    int TRUNK_L = MAP_X(cx - 0.3f * s);
    int TRUNK_R = MAP_X(cx + 0.3f * s);
    int TRUNK_B = MAP_Y(cy - 1.0f * s); // Pangkal di tanah
    int TRUNK_T = MAP_Y(cy + 1.5f * s); // Ketinggian batang

    // === ARSITEKTUR KANOPI DAUN ===
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

void DrawComplexCampfire(float cx, float cy, float scale, bool isOutlineMode, float animTime) {
    float s = scale;

    // --- MATEMATIKA ANGIN (Swaying Effect) ---
    float swayMain = 0.0f, swaySide = 0.0f;
    if (animTime > 0.0f) {
        swayMain = sinf(animTime * 8.0f) * 0.3f * s; 
        swaySide = cosf(animTime * 12.0f) * 0.2f * s;
    }

    // KAYU BAKAR
    float logL_x0 = cx - 1.5f * s, logL_y0 = cy + 0.5f * s;
    float logL_x1 = cx + 1.5f * s, logL_y1 = cy - 0.5f * s;
    
    float logR_x0 = cx - 1.5f * s, logR_y0 = cy - 0.5f * s;
    float logR_x1 = cx + 1.5f * s, logR_y1 = cy + 0.5f * s;

    // TATA LETAK 4 API (Teardrop Geometrics)
    int mainBaseX = MAP_X(cx);
    int mainBaseY = MAP_Y(cy);
    int mainApexX = MAP_X(cx + swayMain); 
    int mainApexY = MAP_Y(cy + 2.5f * s);
    int mainR = (int)(0.8f * s * G_TickStep);

    int leftBaseX = MAP_X(cx - 0.6f * s);
    int leftBaseY = MAP_Y(cy - 0.2f * s);
    int leftApexX = MAP_X(cx - 1.2f * s + swaySide); 
    int leftApexY = MAP_Y(cy + 1.5f * s);
    int leftR = (int)(0.5f * s * G_TickStep);

    int rightBaseX = MAP_X(cx + 0.6f * s);
    int rightBaseY = MAP_Y(cy - 0.2f * s);
    int rightApexX = MAP_X(cx + 1.2f * s + swaySide);
    int rightApexY = MAP_Y(cy + 1.5f * s);
    int rightR = (int)(0.5f * s * G_TickStep);

    int innerBaseX = MAP_X(cx);
    int innerBaseY = MAP_Y(cy);
    int innerApexX = MAP_X(cx + swayMain * 1.2f); 
    int innerApexY = MAP_Y(cy + 1.2f * s);
    int innerR = (int)(0.4f * s * G_TickStep);

    if (isOutlineMode) {
        Color outCol = LIGHTGRAY;
        // Kayu menyilang 
        BresenhamLine(MAP_X(logL_x0), MAP_Y(logL_y0), MAP_X(logL_x1), MAP_Y(logL_y1), DARKBROWN);
        BresenhamLine(MAP_X(logR_x0), MAP_Y(logR_y0), MAP_X(logR_x1), MAP_Y(logR_y1), DARKBROWN);
        
        if (animTime >= 0.0f) {
            DrawFlameTeardrop(mainApexX, mainApexY, mainBaseX, mainBaseY, mainR, outCol, true);
            DrawFlameTeardrop(leftApexX, leftApexY, leftBaseX, leftBaseY, leftR, outCol, true);
            DrawFlameTeardrop(rightApexX, rightApexY, rightBaseX, rightBaseY, rightR, outCol, true);
            DrawFlameTeardrop(innerApexX, innerApexY, innerBaseX, innerBaseY, innerR, outCol, true);
        }
    } else {
        Bres_ThickLine(MAP_X(logL_x0), MAP_Y(logL_y0), MAP_X(logL_x1), MAP_Y(logL_y1), 10, DARKBROWN);
        Bres_ThickLine(MAP_X(logR_x0), MAP_Y(logR_y0), MAP_X(logR_x1), MAP_Y(logR_y1), 10, DARKBROWN);
        
        if (animTime >= 0.0f) {
            DrawFlameTeardrop(mainApexX, mainApexY, mainBaseX, mainBaseY, mainR, RED, false);
            DrawFlameTeardrop(leftApexX, leftApexY, leftBaseX, leftBaseY, leftR, ORANGE, false);
            DrawFlameTeardrop(rightApexX, rightApexY, rightBaseX, rightBaseY, rightR, ORANGE, false);
            DrawFlameTeardrop(innerApexX, innerApexY, innerBaseX, innerBaseY, innerR, YELLOW, false);
        }
    }
}

void DrawStarField(float minX, float maxX, float animTime, float alphaFade, bool isOutlineMode) {
    for (float sx = 100.0f; sx >= -50.0f; sx -= 0.2f) {
        
        if (sx >= minX && sx <= maxX) {
            
            // --- BINTANG 1 (Bentuk '+', Zona Bawah-Menengah) ---
            float sy1 = 2.5f + (fabs(sinf(sx * 17.31f)) * 40.0f); 
            float twinkle1 = 0.3f + (fabs(cosf(animTime * 2.5f + sx)) * 0.7f); 
            unsigned char a1 = isOutlineMode ? 255 : (unsigned char)(255.0f * alphaFade * twinkle1);
            Color c1 = isOutlineMode ? RAYWHITE : (Color){255, 250, 220, a1};
            
            int bx1 = MAP_X(sx);
            int by1 = MAP_Y(sy1);
            
            // Gambar bentuk '+'
            BresenhamLine(bx1 - 1, by1, bx1 + 1, by1, c1);
            BresenhamLine(bx1, by1 - 1, bx1, by1 + 1, c1);

            // --- BINTANG 2 (Bentuk 'x', Zona Menengah-Puncak) ---
            float sy2 = 10.0f + (fabs(cosf(sx * 33.79f)) * 80.0f); 
            float twinkle2 = 0.3f + (fabs(sinf(animTime * 1.8f + sx * 2.0f)) * 0.7f); 
            unsigned char a2 = isOutlineMode ? 255 : (unsigned char)(255.0f * alphaFade * twinkle2);
            Color c2 = isOutlineMode ? RAYWHITE : (Color){255, 250, 220, a2};
            
            int bx2 = MAP_X(sx);
            int by2 = MAP_Y(sy2);
            
            // Gambar bentuk 'x'
            BresenhamLine(bx2 - 1, by2 - 1, bx2 + 1, by2 + 1, c2);
            BresenhamLine(bx2 - 1, by2 + 1, bx2 + 1, by2 - 1, c2);
        }
    }
}

void DrawRainWeather(int screenW, int screenH, float animTime, bool isOutlineMode) {
    DrawRectangle(0, 0, screenW, screenH, Fade(DARKGRAY, 0.3f));

    // Generate 300 rintik hujan murni menggunakan Matematika (Tanpa Array memori)
    for (int i = 0; i < 300; i++) {
        // Acak posisi X di layar menggunakan seed trigonometri 'i'
        int rx = (int)(fabs(sinf(i * 12.9898f)) * screenW);
        
        // Posisi Y jatuh seiring waktu (animTime). 
        // Menggunakan fmodf agar rintik yang menyentuh bawah layar kembali ke atas secara instan!
        float fallSpeed = 800.0f * (0.8f + fabs(cosf(i)) * 0.5f); 
        int ry = (int)fmodf((i * 78.233f) + (animTime * fallSpeed), screenH + 100) - 50;
        
        // Gambar Hujan miring (tertiup angin) dengan Bresenham
        int dropLen = 15 + (int)(fabs(sinf(i)) * 10); 
        Color rainCol = isOutlineMode ? RAYWHITE : (Color){150, 200, 250, 150};
        
        // Garis ditarik dari (rx, ry) miring ke kiri sejauh 5 pixel
        BresenhamLine(rx, ry, rx - 5, ry + dropLen, rainCol);
    }
}