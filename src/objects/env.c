#include "env.h"
#include "coords.h"
#include "src/algo/bresenham.h" // Menggunakan implementasi dosen
#include <stdio.h>

void DrawComplexTree(float cx, float cy, float scale, bool isOutlineMode) {
    // 1. Kalkulasi Dimensi dalam unit Kartesian
    float trunkW = 1.0f * scale; 
    float trunkH = 2.0f * scale; 
    float leavesW = 3.0f * scale; 
    float leavesH = 3.0f * scale; 

    // 2. Tentukan Titik-Titik Geometri (Hierarchical Decomposition)
    // Batang (persegi panjang)
    float x_b0 = cx - (trunkW / 2); // Kiri batang
    float y_b0 = cy;                // Bawah batang
    float x_b1 = cx + (trunkW / 2); // Kanan batang
    float y_b1 = cy + trunkH;       // Atas batang

    // Daun (segitiga sama kaki)
    float x_d0 = cx - (leavesW / 2);    // Kiri bawah daun
    float y_d0 = cy + trunkH;           // Garis bawah daun
    float x_d1 = cx + (leavesW / 2);    // Kanan bawah daun
    float x_d2 = cx;                    // Puncak daun tengah
    float y_d2 = cy + trunkH + leavesH; // Tinggi puncak daun

    if (isOutlineMode) {
        // --- MODE BLUEPRINT (Hanya Outline) ---
        Color outlineColor = LIGHTGRAY;
        
        // Gambar Batang
        BresenhamLine(CS_X(x_b0), CS_Y(y_b0), CS_X(x_b1), CS_Y(y_b0), outlineColor); // Bawah
        BresenhamLine(CS_X(x_b1), CS_Y(y_b0), CS_X(x_b1), CS_Y(y_b1), outlineColor); // Kanan
        BresenhamLine(CS_X(x_b1), CS_Y(y_b1), CS_X(x_b0), CS_Y(y_b1), outlineColor); // Atas
        BresenhamLine(CS_X(x_b0), CS_Y(y_b1), CS_X(x_b0), CS_Y(y_b0), outlineColor); // Kiri

        // Gambar Daun
        BresenhamLine(CS_X(x_d0), CS_Y(y_d0), CS_X(x_d1), CS_Y(y_d0), outlineColor); // Bawah
        BresenhamLine(CS_X(x_d1), CS_Y(y_d0), CS_X(x_d2), CS_Y(y_d2), outlineColor); // Kanan
        BresenhamLine(CS_X(x_d2), CS_Y(y_d2), CS_X(x_d0), CS_Y(y_d0), outlineColor); // Kiri

        // Teks Koordinat Puncak
        char coordsText[20];
        snprintf(coordsText, 20, "(%.1f, %.1f)", cx, cy + trunkH + leavesH);
        DrawText(coordsText, CS_X(x_d2) - 15, CS_Y(y_d2) - 15, 10, outlineColor);

    } else {
        // --- MODE PENUH (SCANLINE FILLING MENGGUNAKAN BRESENHAM) ---
        
        // 1. Mewarnai Batang (Rectangle Scanline)
        // Kita ubah dulu batas atas dan bawah ke koordinat layar
        int sy_b_top = CS_Y(y_b1);    // Nilai Y layar lebih kecil di atas
        int sy_b_bottom = CS_Y(y_b0); // Nilai Y layar lebih besar di bawah
        int sx_b_left = CS_X(x_b0);
        int sx_b_right = CS_X(x_b1);

        // Tarik garis horizontal dari atas ke bawah batang
        for (int y = sy_b_top; y <= sy_b_bottom; y++) {
            BresenhamLine(sx_b_left, y, sx_b_right, y, BROWN);
        }

        // 2. Mewarnai Daun (Triangle Scanline Interpolation)
        int sy_d_top = CS_Y(y_d2);
        int sy_d_bottom = CS_Y(y_d0);
        int sx_d_top = CS_X(x_d2);
        int sx_d_left = CS_X(x_d0);
        int sx_d_right = CS_X(x_d1);

        // Hindari pembagian dengan nol jika tinggi daun 0
        int deltaY = sy_d_bottom - sy_d_top;
        if (deltaY > 0) {
            // Tarik garis horizontal dari pucuk daun hingga ke alas
            for (int y = sy_d_top; y <= sy_d_bottom; y++) {
                // Interpolasi linear: mencari jarak proporsional 't' dari rentang 0.0 hingga 1.0
                float t = (float)(y - sy_d_top) / (float)deltaY;
                
                // Mencari batas kiri dan batas kanan x pada titik Y saat ini
                int currentX_left = sx_d_top + (int)(t * (sx_d_left - sx_d_top));
                int currentX_right = sx_d_top + (int)(t * (sx_d_right - sx_d_top));
                
                BresenhamLine(currentX_left, y, currentX_right, y, GREEN);
            }
        }
    }
}

void DrawComplexCampfire(float cx, float cy, float scale, bool isOutlineMode) {
    // Kayu Bakar (Menyilang)
    float logL_x0 = cx - 1.5f * scale, logL_y0 = cy + 0.5f * scale;
    float logL_x1 = cx + 1.5f * scale, logL_y1 = cy - 0.5f * scale;
    
    float logR_x0 = cx - 1.5f * scale, logR_y0 = cy - 0.5f * scale;
    float logR_x1 = cx + 1.5f * scale, logR_y1 = cy + 0.5f * scale;

    // Api Utama (Segitiga)
    float f_x0 = cx - 1.0f * scale, f_y0 = cy;
    float f_x1 = cx + 1.0f * scale, f_y1 = cy;
    float f_x2 = cx, f_y2 = cy + 2.5f * scale; // Puncak api

    if (isOutlineMode) {
        Color outCol = LIGHTGRAY;
        // Kayu menyilang
        BresenhamLine(CS_X(logL_x0), CS_Y(logL_y0), CS_X(logL_x1), CS_Y(logL_y1), DARKBROWN);
        BresenhamLine(CS_X(logR_x0), CS_Y(logR_y0), CS_X(logR_x1), CS_Y(logR_y1), DARKBROWN);
        
        // Api
        BresenhamLine(CS_X(f_x0), CS_Y(f_y0), CS_X(f_x1), CS_Y(f_y1), outCol);
        BresenhamLine(CS_X(f_x1), CS_Y(f_y1), CS_X(f_x2), CS_Y(f_y2), outCol);
        BresenhamLine(CS_X(f_x2), CS_Y(f_y2), CS_X(f_x0), CS_Y(f_y0), outCol);
        
        char txt[30]; snprintf(txt, 30, "Api(%.1f, %.1f)", cx, cy + 2.5f*scale);
        DrawText(txt, CS_X(f_x2)-20, CS_Y(f_y2)-15, 10, ORANGE);
    } else {
        // Menggunakan garis tebal buatan dosen untuk kayu
        Bres_ThickLine(CS_X(logL_x0), CS_Y(logL_y0), CS_X(logL_x1), CS_Y(logL_y1), 8, DARKBROWN);
        Bres_ThickLine(CS_X(logR_x0), CS_Y(logR_y0), CS_X(logR_x1), CS_Y(logR_y1), 8, DARKBROWN);
        
        // Scanline untuk Api (Merah/Oranye)
        int sy_top = CS_Y(f_y2), sy_bot = CS_Y(f_y0);
        int sx_top = CS_X(f_x2), sx_left = CS_X(f_x0), sx_right = CS_X(f_x1);
        int dy = sy_bot - sy_top;
        if (dy > 0) {
            for (int y = sy_top; y <= sy_bot; y++) {
                float t = (float)(y - sy_top) / dy;
                int xl = sx_top + (int)(t * (sx_left - sx_top));
                int xr = sx_top + (int)(t * (sx_right - sx_top));
                // Efek gradasi sederhana berbasis Y
                Color fireCol = (y < sy_top + dy/2) ? YELLOW : RED; 
                BresenhamLine(xl, y, xr, y, fireCol);
            }
        }
    }
}