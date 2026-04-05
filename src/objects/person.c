#include "person.h"
#include "coords.h"
#include "src/algo/bresenham.h"
#include "src/algo/midcircle.h" 
#include <math.h>
#include <stdio.h>

static void FillRect(int xL, int xR, int yT, int yB, Color c) {
    for (int y = yT; y <= yB; y++) BresenhamLine(xL, y, xR, y, c);
}

// Hanya menggambar garis luar kubah 
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

void DrawComplexPerson(float cx, float cy, float scale, bool isOutlineMode, float animTime) {
    float s = scale;

    // --- MATEMATIKA ANIMASI ---
    float walkCycle = 0.0f;
    float breath = 0.0f;
    
    // Logika Berjalan vs Idle (Berhenti di t = 5.0s)
    if (animTime > 0.0f && animTime <= 5.0f) {
        // Berjalan: Kaki mengayun seperti pendulum (Sinusoidal)
        walkCycle = sinf(animTime * 12.0f) * 0.4f * s; 
    } else if (animTime > 5.0f) {
        // Idle/Diam: Bahu dan dada naik turun sedikit seakan bernapas
        breath = sinf(animTime * 3.0f) * 0.04f * s;
    }

    // === ARSITEKTUR KINEMATIKA ===
    int Y_GROUND   = MAP_Y(cy - 1.0f * s); 
    int Y_KNEE     = MAP_Y(cy - 0.2f * s);
    int Y_HIP      = MAP_Y(cy + 0.6f * s);
    // Tambahkan variabel "breath" pada sendi atas
    int Y_CHEST    = MAP_Y(cy + 1.3f * s + breath); 
    int Y_SHOULDER = MAP_Y(cy + 1.8f * s + breath);
    int Y_ELBOW    = MAP_Y(cy + 1.1f * s + breath);
    int Y_HAND     = MAP_Y(cy + 0.4f * s + breath);
    int Y_HEAD     = MAP_Y(cy + 2.5f * s + breath);

    int X_C        = MAP_X(cx);                 
    int X_HIP_L    = MAP_X(cx - 0.2f * s); 
    int X_HIP_R    = MAP_X(cx + 0.2f * s);      
    
    // Kaki diayunkan berlawanan arah menggunakan walkCycle
    int X_FOOT_L   = MAP_X(cx - 0.2f * s + walkCycle);      
    int X_FOOT_R   = MAP_X(cx + 0.2f * s - walkCycle);      
    
    // Tangan ikut berayun berlawanan dengan kaki
    int X_SHL_L    = MAP_X(cx - 0.5f * s);      
    int X_SHL_R    = MAP_X(cx + 0.5f * s);      
    int X_ARM_L    = MAP_X(cx - 0.7f * s - walkCycle);      
    int X_ARM_R    = MAP_X(cx + 0.7f * s + walkCycle);      

    int R_HEAD     = (int)(0.4f * s * G_TickStep);
    int R_JOINT    = (int)(0.15f * s * G_TickStep);

    Color skinCol  = (Color){240, 190, 150, 255}; 
    Color shirtCol = (Color){40, 140, 100, 255};  
    Color pantCol  = (Color){90, 70, 50, 255};    
    Color shoeCol  = (Color){30, 30, 30, 255};    
    Color hatCol   = (Color){139, 69, 19, 255};   
    Color strapCol = (Color){20, 20, 20, 255};    
    Color lineCol  = RAYWHITE;

    if (isOutlineMode) {
        // Tulang Kaki & Outline Sepatu
        BresenhamLine(X_HIP_L, Y_HIP, X_HIP_L, Y_KNEE, lineCol);
        BresenhamLine(X_HIP_L, Y_KNEE, X_FOOT_L, Y_GROUND, lineCol);
        DrawTopHalfArc(X_FOOT_L, Y_GROUND, R_JOINT + 2, lineCol); // Outline sepatu (Setengah lingkaran)
        BresenhamLine(X_FOOT_L - 8, Y_GROUND, X_FOOT_L + 8, Y_GROUND, lineCol); // Sol sepatu

        BresenhamLine(X_HIP_R, Y_HIP, X_HIP_R, Y_KNEE, lineCol);
        BresenhamLine(X_HIP_R, Y_KNEE, X_FOOT_R, Y_GROUND, lineCol);
        DrawTopHalfArc(X_FOOT_R, Y_GROUND, R_JOINT + 2, lineCol); 
        BresenhamLine(X_FOOT_R - 8, Y_GROUND, X_FOOT_R + 8, Y_GROUND, lineCol); 

        // Tulang Lengan
        BresenhamLine(X_SHL_L, Y_SHOULDER, X_ARM_L, Y_ELBOW, lineCol);
        BresenhamLine(X_ARM_L, Y_ELBOW, X_ARM_L, Y_HAND, lineCol);
        BresenhamLine(X_SHL_R, Y_SHOULDER, X_ARM_R, Y_ELBOW, lineCol);
        BresenhamLine(X_ARM_R, Y_ELBOW, X_ARM_R, Y_HAND, lineCol);

        // Badan (Torso Bounding Box)
        BresenhamLine(X_SHL_L, Y_SHOULDER, X_SHL_R, Y_SHOULDER, lineCol); 
        BresenhamLine(X_SHL_R, Y_SHOULDER, X_SHL_R, Y_HIP, lineCol);      
        BresenhamLine(X_SHL_R, Y_HIP, X_SHL_L, Y_HIP, lineCol);           
        BresenhamLine(X_SHL_L, Y_HIP, X_SHL_L, Y_SHOULDER, lineCol);      

        // Kepala & Leher
        BresenhamLine(X_C, Y_SHOULDER, X_C, Y_HEAD, lineCol); 
        Midcircle(X_C, Y_HEAD, R_HEAD, lineCol);

        // Outline Topi 
        DrawTopHalfArc(X_C, Y_HEAD - 2, R_HEAD + 2, lineCol); 
        BresenhamLine(X_C - R_HEAD - 10, Y_HEAD - 2, X_C + R_HEAD + 10, Y_HEAD - 2, lineCol); 

        // Outline Tali Ransel
        BresenhamLine(X_SHL_L + 8, Y_SHOULDER, X_SHL_L + 8, Y_CHEST, lineCol); 
        BresenhamLine(X_SHL_L + 8, Y_CHEST, X_SHL_L, Y_CHEST + 12, lineCol);   
        BresenhamLine(X_SHL_R - 8, Y_SHOULDER, X_SHL_R - 8, Y_CHEST, lineCol); 
        BresenhamLine(X_SHL_R - 8, Y_CHEST, X_SHL_R, Y_CHEST + 12, lineCol);   

        Midcircle(X_HIP_L, Y_KNEE, R_JOINT, YELLOW);
        Midcircle(X_HIP_R, Y_KNEE, R_JOINT, YELLOW);
        Midcircle(X_ARM_L, Y_ELBOW, R_JOINT, YELLOW);
        Midcircle(X_ARM_R, Y_ELBOW, R_JOINT, YELLOW);
        Midcircle(X_ARM_L, Y_HAND, R_JOINT, YELLOW); // Kepalan Kiri
        Midcircle(X_ARM_R, Y_HAND, R_JOINT, YELLOW); // Kepalan Kanan

    } else {
        // Kaki Kargo
        int legThick = (int)(0.35f * s * G_TickStep);
        Bres_ThickLine(X_HIP_L, Y_HIP, X_HIP_L, Y_KNEE, legThick, pantCol);
        Bres_ThickLine(X_HIP_L, Y_KNEE, X_FOOT_L, Y_GROUND, legThick, pantCol);
        Bres_ThickLine(X_HIP_R, Y_HIP, X_HIP_R, Y_KNEE, legThick, pantCol);
        Bres_ThickLine(X_HIP_R, Y_KNEE, X_FOOT_R, Y_GROUND, legThick, pantCol);
        
        // Sepatu Bot
        FillTopHalfCircle(X_FOOT_L, Y_GROUND, R_JOINT + 2, shoeCol);
        FillTopHalfCircle(X_FOOT_R, Y_GROUND, R_JOINT + 2, shoeCol);
        Bres_ThickLine(X_FOOT_L - 8, Y_GROUND, X_FOOT_L + 8, Y_GROUND, 4, shoeCol);
        Bres_ThickLine(X_FOOT_R - 8, Y_GROUND, X_FOOT_R + 8, Y_GROUND, 4, shoeCol);

        // Badan 
        FillRect(X_SHL_L, X_SHL_R, Y_SHOULDER, Y_HIP, shirtCol);
        
        // Tali Ransel
        Bres_ThickLine(X_SHL_L + 8, Y_SHOULDER, X_SHL_L + 8, Y_CHEST, 4, strapCol);
        Bres_ThickLine(X_SHL_L + 8, Y_CHEST, X_SHL_L, Y_CHEST + 12, 4, strapCol);
        Bres_ThickLine(X_SHL_R - 8, Y_SHOULDER, X_SHL_R - 8, Y_CHEST, 4, strapCol);
        Bres_ThickLine(X_SHL_R - 8, Y_CHEST, X_SHL_R, Y_CHEST + 12, 4, strapCol);

        // Kepala & Leher
        Bres_ThickLine(X_C, Y_SHOULDER, X_C, Y_HEAD, 10, skinCol); 
        MidcircleFilled(X_C, Y_HEAD, R_HEAD, skinCol);

        // Topi Bucket Cokelat
        FillTopHalfCircle(X_C, Y_HEAD - 2, R_HEAD + 2, hatCol);
        Bres_ThickLine(X_C - R_HEAD - 10, Y_HEAD - 2, X_C + R_HEAD + 10, Y_HEAD - 2, 6, hatCol);

        // Lengan 
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