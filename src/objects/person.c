#include "person.h"
#include "coords.h"
#include "src/algo/bresenham.h"
#include "src/algo/midcircle.h" 
#include <stdio.h>
#include <raylib.h> // Diperlukan untuk DrawPixel di fungsi pembantu

// Makro pemetaan presisi
#define MAP_X(x) (G_OriginX + (int)((x) * G_TickStep))
#define MAP_Y(y) (G_OriginY - (int)((y) * G_TickStep))

// Helper untuk mewarnai balok menggunakan Bresenham
static void FillRect(int xL, int xR, int yT, int yB, Color c) {
    for (int y = yT; y <= yB; y++) BresenhamLine(xL, y, xR, y, c);
}

// =====================================================================
// FUNGSI MATEMATIKA BARU: Setengah Lingkaran Atas (Modifikasi Midpoint)
// =====================================================================
// 1. Hanya menggambar garis luar kubah (Untuk Mode Blueprint)
static void DrawTopHalfArc(int cx, int cy, int r, Color c) {
    int x = 0, y = r;
    int d = 3 - 2 * r;
    while (y >= x) {
        // Hanya memproses octant yang nilai Y-nya dikurangi (naik ke atas layar)
        DrawPixel(cx + x, cy - y, c); 
        DrawPixel(cx - x, cy - y, c); 
        DrawPixel(cx + y, cy - x, c); 
        DrawPixel(cx - y, cy - x, c); 
        
        if (d < 0) d += 4 * x + 6;
        else { d += 4 * (x - y) + 10; y--; }
        x++;
    }
}

// 2. Mewarnai kubah dengan scanline horizontal (Untuk Mode Penuh)
static void FillTopHalfCircle(int cx, int cy, int r, Color c) {
    int x = 0, y = r;
    int d = 3 - 2 * r;
    while (y >= x) {
        // Menarik garis horizontal menyapu belahan atas lingkaran
        BresenhamLine(cx - x, cy - y, cx + x, cy - y, c);
        BresenhamLine(cx - y, cy - x, cx + y, cy - x, c);
        
        if (d < 0) d += 4 * x + 6;
        else { d += 4 * (x - y) + 10; y--; }
        x++;
    }
}
// =====================================================================

void DrawComplexPerson(float cx, float cy, float scale, bool isOutlineMode) {
    float s = scale;

    // === 1. ARSITEKTUR KINEMATIKA (SKELETAL JOINTS) ===
    
    // Sendi Utama (Y)
    int Y_GROUND   = MAP_Y(cy - 1.0f * s); 
    int Y_KNEE     = MAP_Y(cy - 0.2f * s);
    int Y_HIP      = MAP_Y(cy + 0.6f * s);
    int Y_CHEST    = MAP_Y(cy + 1.3f * s); // Titik belok tali ransel
    int Y_SHOULDER = MAP_Y(cy + 1.8f * s);
    int Y_ELBOW    = MAP_Y(cy + 1.1f * s);
    int Y_HAND     = MAP_Y(cy + 0.4f * s);
    int Y_HEAD     = MAP_Y(cy + 2.5f * s);

    // Sendi Utama (X)
    int X_C        = MAP_X(cx);                 // Tulang Belakang (Tengah)
    int X_HIP_L    = MAP_X(cx - 0.3f * s);      // Pinggul Kiri
    int X_HIP_R    = MAP_X(cx + 0.3f * s);      // Pinggul Kanan
    int X_FOOT_L   = MAP_X(cx - 0.4f * s);      // Pijakan Kiri
    int X_FOOT_R   = MAP_X(cx + 0.4f * s);      // Pijakan Kanan
    int X_SHL_L    = MAP_X(cx - 0.5f * s);      // Bahu Kiri
    int X_SHL_R    = MAP_X(cx + 0.5f * s);      // Bahu Kanan
    int X_ARM_L    = MAP_X(cx - 0.7f * s);      // Siku/Tangan Kiri
    int X_ARM_R    = MAP_X(cx + 0.7f * s);      // Siku/Tangan Kanan

    // Ukuran Geometri
    int R_HEAD     = (int)(0.4f * s * G_TickStep);
    int R_JOINT    = (int)(0.15f * s * G_TickStep); // Jari-jari sendi/siku

    // Palet Warna Pakaian Camper
    Color skinCol  = (Color){240, 190, 150, 255}; 
    Color shirtCol = (Color){40, 140, 100, 255};  
    Color pantCol  = (Color){90, 70, 50, 255};    
    Color shoeCol  = (Color){30, 30, 30, 255};    
    Color hatCol   = (Color){139, 69, 19, 255};   // Topi cokelat tua
    Color strapCol = (Color){20, 20, 20, 255};    
    Color lineCol  = RAYWHITE;

    if (isOutlineMode) {
        // --- MODE BLUEPRINT (Wireframe Artikulasi Lengkap) ---
        
        // 1. Tulang Kaki & Outline Sepatu
        BresenhamLine(X_HIP_L, Y_HIP, X_HIP_L, Y_KNEE, lineCol);
        BresenhamLine(X_HIP_L, Y_KNEE, X_FOOT_L, Y_GROUND, lineCol);
        DrawTopHalfArc(X_FOOT_L, Y_GROUND, R_JOINT + 2, lineCol); // Outline sepatu (Setengah lingkaran)
        BresenhamLine(X_FOOT_L - 8, Y_GROUND, X_FOOT_L + 8, Y_GROUND, lineCol); // Sol sepatu

        BresenhamLine(X_HIP_R, Y_HIP, X_HIP_R, Y_KNEE, lineCol);
        BresenhamLine(X_HIP_R, Y_KNEE, X_FOOT_R, Y_GROUND, lineCol);
        DrawTopHalfArc(X_FOOT_R, Y_GROUND, R_JOINT + 2, lineCol); 
        BresenhamLine(X_FOOT_R - 8, Y_GROUND, X_FOOT_R + 8, Y_GROUND, lineCol); 

        // 2. Tulang Lengan
        BresenhamLine(X_SHL_L, Y_SHOULDER, X_ARM_L, Y_ELBOW, lineCol);
        BresenhamLine(X_ARM_L, Y_ELBOW, X_ARM_L, Y_HAND, lineCol);
        BresenhamLine(X_SHL_R, Y_SHOULDER, X_ARM_R, Y_ELBOW, lineCol);
        BresenhamLine(X_ARM_R, Y_ELBOW, X_ARM_R, Y_HAND, lineCol);

        // 3. Badan (Torso Bounding Box)
        BresenhamLine(X_SHL_L, Y_SHOULDER, X_SHL_R, Y_SHOULDER, lineCol); 
        BresenhamLine(X_SHL_R, Y_SHOULDER, X_SHL_R, Y_HIP, lineCol);      
        BresenhamLine(X_SHL_R, Y_HIP, X_SHL_L, Y_HIP, lineCol);           
        BresenhamLine(X_SHL_L, Y_HIP, X_SHL_L, Y_SHOULDER, lineCol);      

        // 4. Kepala & Leher
        BresenhamLine(X_C, Y_SHOULDER, X_C, Y_HEAD, lineCol); 
        Midcircle(X_C, Y_HEAD, R_HEAD, lineCol);

        // 5. Outline Topi (Menggunakan Setengah Lingkaran agar wajah tidak tertutup!)
        DrawTopHalfArc(X_C, Y_HEAD - 2, R_HEAD + 2, lineCol); 
        BresenhamLine(X_C - R_HEAD - 10, Y_HEAD - 2, X_C + R_HEAD + 10, Y_HEAD - 2, lineCol); 

        // 6. Outline Tali Ransel
        BresenhamLine(X_SHL_L + 8, Y_SHOULDER, X_SHL_L + 8, Y_CHEST, lineCol); 
        BresenhamLine(X_SHL_L + 8, Y_CHEST, X_SHL_L, Y_CHEST + 12, lineCol);   
        BresenhamLine(X_SHL_R - 8, Y_SHOULDER, X_SHL_R - 8, Y_CHEST, lineCol); 
        BresenhamLine(X_SHL_R - 8, Y_CHEST, X_SHL_R, Y_CHEST + 12, lineCol);   

        // 7. Titik Sendi Engsel (Ditambahkan Sendi Tangan yang kelupaan!)
        Midcircle(X_HIP_L, Y_KNEE, R_JOINT, YELLOW);
        Midcircle(X_HIP_R, Y_KNEE, R_JOINT, YELLOW);
        Midcircle(X_ARM_L, Y_ELBOW, R_JOINT, YELLOW);
        Midcircle(X_ARM_R, Y_ELBOW, R_JOINT, YELLOW);
        Midcircle(X_ARM_L, Y_HAND, R_JOINT, YELLOW); // Kepalan Kiri
        Midcircle(X_ARM_R, Y_HAND, R_JOINT, YELLOW); // Kepalan Kanan

        char txt[30]; snprintf(txt, 30, "Skeletal Root");
        DrawText(txt, X_C - 30, Y_HIP + 10, 10, ORANGE);

    } else {
        // --- MODE PENUH (Rendering Bervolume) ---

        // 1. Kaki Kargo
        int legThick = (int)(0.35f * s * G_TickStep);
        Bres_ThickLine(X_HIP_L, Y_HIP, X_HIP_L, Y_KNEE, legThick, pantCol);
        Bres_ThickLine(X_HIP_L, Y_KNEE, X_FOOT_L, Y_GROUND, legThick, pantCol);
        Bres_ThickLine(X_HIP_R, Y_HIP, X_HIP_R, Y_KNEE, legThick, pantCol);
        Bres_ThickLine(X_HIP_R, Y_KNEE, X_FOOT_R, Y_GROUND, legThick, pantCol);
        
        // Sepatu Bot (Memakai FillTopHalfCircle agar terlihat membumi)
        FillTopHalfCircle(X_FOOT_L, Y_GROUND, R_JOINT + 2, shoeCol);
        FillTopHalfCircle(X_FOOT_R, Y_GROUND, R_JOINT + 2, shoeCol);
        Bres_ThickLine(X_FOOT_L - 8, Y_GROUND, X_FOOT_L + 8, Y_GROUND, 4, shoeCol);
        Bres_ThickLine(X_FOOT_R - 8, Y_GROUND, X_FOOT_R + 8, Y_GROUND, 4, shoeCol);

        // 2. Badan (Kaus Camper)
        FillRect(X_SHL_L, X_SHL_R, Y_SHOULDER, Y_HIP, shirtCol);
        
        // Tali Ransel
        Bres_ThickLine(X_SHL_L + 8, Y_SHOULDER, X_SHL_L + 8, Y_CHEST, 4, strapCol);
        Bres_ThickLine(X_SHL_L + 8, Y_CHEST, X_SHL_L, Y_CHEST + 12, 4, strapCol);
        Bres_ThickLine(X_SHL_R - 8, Y_SHOULDER, X_SHL_R - 8, Y_CHEST, 4, strapCol);
        Bres_ThickLine(X_SHL_R - 8, Y_CHEST, X_SHL_R, Y_CHEST + 12, 4, strapCol);

        // 3. Kepala & Leher
        Bres_ThickLine(X_C, Y_SHOULDER, X_C, Y_HEAD, 10, skinCol); 
        MidcircleFilled(X_C, Y_HEAD, R_HEAD, skinCol);

        // 4. Topi Bucket Cokelat (Menggunakan FillTopHalfCircle)
        FillTopHalfCircle(X_C, Y_HEAD - 2, R_HEAD + 2, hatCol);
        Bres_ThickLine(X_C - R_HEAD - 10, Y_HEAD - 2, X_C + R_HEAD + 10, Y_HEAD - 2, 6, hatCol);

        // 5. Lengan (Lapis terdepan)
        int armThick = (int)(0.25f * s * G_TickStep);
        // Kiri
        Bres_ThickLine(X_SHL_L, Y_SHOULDER, X_ARM_L, Y_ELBOW, armThick, shirtCol);
        MidcircleFilled(X_ARM_L, Y_ELBOW, R_JOINT, skinCol); 
        Bres_ThickLine(X_ARM_L, Y_ELBOW, X_ARM_L, Y_HAND, armThick, skinCol);
        MidcircleFilled(X_ARM_L, Y_HAND, R_JOINT, skinCol);  // Tangan (Kepalan) Kiri
        
        // Kanan
        Bres_ThickLine(X_SHL_R, Y_SHOULDER, X_ARM_R, Y_ELBOW, armThick, shirtCol);
        MidcircleFilled(X_ARM_R, Y_ELBOW, R_JOINT, skinCol); 
        Bres_ThickLine(X_ARM_R, Y_ELBOW, X_ARM_R, Y_HAND, armThick, skinCol);
        MidcircleFilled(X_ARM_R, Y_HAND, R_JOINT, skinCol);  // Tangan (Kepalan) Kanan
    }
}