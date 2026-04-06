#include "bear.h"
#include "coords.h"
#include "src/algo/bresenham.h"
#include "src/algo/midcircle.h" 
#include <math.h>
#include <stdio.h>

// Fungsi FillRect menggunakan Bresenham Murni (Sesuai komitmen kita!)
static void FillRect(int xL, int xR, int yT, int yB, Color c) {
    for (int y = yT; y <= yB; y++) BresenhamLine(xL, y, xR, y, c);
}

void DrawComplexBear(float cx, float cy, float scale, bool isOutlineMode, float animTime) {
    float s = scale;
    // Gelombang berjalan untuk kaki (Ayunan pendulum)
    float walkCycle = sinf(animTime * 10.0f) * 0.4f * s;
    
    // Gelombang untuk kepala (Kepala beruang mengangguk berat saat berjalan)
    float headBob = fabs(sinf(animTime * 10.0f)) * 0.15f * s;

    // === ARSITEKTUR SUMBU Y ===
    int Y_GROUND = MAP_Y(cy - 1.0f * s);
    int Y_BELLY  = MAP_Y(cy - 0.2f * s);
    int Y_BACK   = MAP_Y(cy + 0.8f * s);
    int Y_HEAD   = MAP_Y(cy + 0.6f * s + headBob);
    int Y_SNOUT  = MAP_Y(cy + 0.4f * s + headBob);
    int Y_EAR    = MAP_Y(cy + 1.0f * s + headBob);

    // === ARSITEKTUR SUMBU X (Bodi Bulky) ===
    int X_RUMP  = MAP_X(cx - 1.4f * s); // Bokong (Belakang)
    int X_CHEST = MAP_X(cx + 1.2f * s); // Dada (Depan)
    int X_HEAD  = MAP_X(cx + 1.7f * s);
    int X_SNOUT = MAP_X(cx + 2.2f * s);
    int X_EAR   = MAP_X(cx + 1.4f * s);

    // Titik Sendi Kaki (Atas)
    int X_LEG_BL = MAP_X(cx - 1.1f * s); // Back Left
    int X_LEG_BR = MAP_X(cx - 0.7f * s); // Back Right
    int X_LEG_FL = MAP_X(cx + 0.6f * s); // Front Left
    int X_LEG_FR = MAP_X(cx + 1.0f * s); // Front Right

    // Titik Telapak Kaki (Bawah) - Berayun berlawanan!
    int X_FOOT_BL = MAP_X(cx - 1.1f * s - walkCycle);
    int X_FOOT_BR = MAP_X(cx - 0.7f * s + walkCycle); // Pasangan diagonal
    int X_FOOT_FL = MAP_X(cx + 0.6f * s + walkCycle); // Pasangan diagonal
    int X_FOOT_FR = MAP_X(cx + 1.0f * s - walkCycle);

    // Ukuran komponen
    int R_HEAD  = (int)(0.5f * s * G_TickStep);
    int R_EAR   = (int)(0.15f * s * G_TickStep);
    int R_SNOUT = (int)(0.25f * s * G_TickStep);
    int R_BODY_EDGE = (int)(0.5f * s * G_TickStep); // Untuk membulatkan ujung badan
    int LEG_THICK = (int)(0.4f * s * G_TickStep);

    Color furDark  = (Color){60, 40, 20, 255};  // Cokelat sangat gelap (Kaki belakang)
    Color furLight = (Color){90, 60, 30, 255};  // Cokelat terang (Badan & Kaki depan)
    Color snoutCol = (Color){200, 160, 120, 255}; // Krem (Moncong)
    Color lineCol  = RAYWHITE;

    if (isOutlineMode) {
        // Kaki Lapis Belakang
        BresenhamLine(X_LEG_BR, Y_BELLY, X_FOOT_BR, Y_GROUND, lineCol);
        BresenhamLine(X_LEG_FL, Y_BELLY, X_FOOT_FL, Y_GROUND, lineCol);

        // Bodi Utama
        BresenhamLine(X_RUMP, Y_BELLY, X_CHEST, Y_BELLY, lineCol);
        BresenhamLine(X_RUMP, Y_BACK, X_CHEST, Y_BACK, lineCol);
        BresenhamLine(X_RUMP, Y_BACK, X_RUMP, Y_BELLY, lineCol);
        BresenhamLine(X_CHEST, Y_BACK, X_CHEST, Y_BELLY, lineCol);
        Midcircle(X_RUMP, MAP_Y(cy + 0.3f * s), R_BODY_EDGE, lineCol);
        Midcircle(X_CHEST, MAP_Y(cy + 0.3f * s), R_BODY_EDGE, lineCol);

        // Kaki Lapis Depan
        BresenhamLine(X_LEG_BL, Y_BELLY, X_FOOT_BL, Y_GROUND, lineCol);
        BresenhamLine(X_LEG_FR, Y_BELLY, X_FOOT_FR, Y_GROUND, lineCol);

        // Kepala
        Midcircle(X_HEAD, Y_HEAD, R_HEAD, lineCol);
        Midcircle(X_SNOUT, Y_SNOUT, R_SNOUT, lineCol);
        Midcircle(X_EAR, Y_EAR, R_EAR, lineCol);

        MidcircleFilled(X_SNOUT + 4, Y_SNOUT - 4, 3, lineCol); 
    } else {
        // Painter's Algorithm: Gambar dari objek paling jauh (belakang) ke depan
        
        // 1. Kaki Lapis Belakang (Warna Lebih Gelap / Shadow)
        Bres_ThickLine(X_LEG_BR, Y_BELLY, X_FOOT_BR, Y_GROUND, LEG_THICK, furDark);
        Bres_ThickLine(X_LEG_FL, Y_BELLY, X_FOOT_FL, Y_GROUND, LEG_THICK, furDark);

        // 2. Bodi Utama (Kotak dengan ujung membulat)
        FillRect(X_RUMP, X_CHEST, Y_BACK, Y_BELLY, furLight);
        MidcircleFilled(X_RUMP, MAP_Y(cy + 0.3f * s), R_BODY_EDGE, furLight); // Bokong membulat
        MidcircleFilled(X_CHEST, MAP_Y(cy + 0.3f * s), R_BODY_EDGE, furLight); // Dada membulat

        // 3. Kaki Lapis Depan (Warna Terang)
        Bres_ThickLine(X_LEG_BL, Y_BELLY, X_FOOT_BL, Y_GROUND, LEG_THICK, furLight);
        Bres_ThickLine(X_LEG_FR, Y_BELLY, X_FOOT_FR, Y_GROUND, LEG_THICK, furLight);

        // 4. Kepala & Anatomi Wajah
        MidcircleFilled(X_EAR, Y_EAR, R_EAR, furDark); // Telinga
        MidcircleFilled(X_HEAD, Y_HEAD, R_HEAD, furLight); // Kepala utama
        MidcircleFilled(X_SNOUT, Y_SNOUT, R_SNOUT, snoutCol); // Moncong
        
        // Hidung (Titik Hitam kecil di ujung moncong)
        MidcircleFilled(X_SNOUT + 4, Y_SNOUT - 4, 3, BLACK); 
    }
}