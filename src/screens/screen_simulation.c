#include "screen_simulation.h"
#include "coords.h"
#include "screen_type.h"
#include "src/algo/bresenham.h" 
#include "src/objects/env.h"    
#include "src/objects/tent.h"
#include "src/objects/person.h"
#include "src/objects/car.h"

#include <stdio.h>

// Variabel Waktu Global Lokal untuk Simulasi
static float simTimer = 0.0f; 
static bool isSimRunning = true;

// Helper: Interpolasi Warna Linear (LERP)
static Color LerpColor(Color a, Color b, float t) {
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;
    return (Color){
        (unsigned char)(a.r + (b.r - a.r) * t),
        (unsigned char)(a.g + (b.g - a.g) * t),
        (unsigned char)(a.b + (b.b - a.b) * t),
        255
    };
}

void DrawSimulationScreen(UIButton btnBack, int* currentScreen) {
    // 1. UPDATE TIMER
    if (isSimRunning) {
        simTimer += GetFrameTime(); // Menambah waktu sesuai FPS
    }

    // 2. SKENARIO WAKTU & WARNA LANGIT
    Color skySiang = (Color){135, 206, 235, 255}; // Sky Blue
    Color skySenja = (Color){255, 140,  0, 255};  // Dark Orange
    Color skyMalam = (Color){ 10,  15,  30, 255}; // Biru Sangat Gelap

    Color currentSky;
    float phase;

    // Transisi: Siang (0-4s) -> Senja (4-7s) -> Malam (7s+)
    if (simTimer < 4.0f) {
        currentSky = skySiang; // Diam di siang hari
    } else if (simTimer >= 4.0f && simTimer < 7.0f) {
        phase = (simTimer - 4.0f) / 3.0f; // Normalisasi 0.0 sampai 1.0
        currentSky = LerpColor(skySiang, skySenja, phase); // Berubah ke oranye
    } else if (simTimer >= 7.0f && simTimer < 10.0f) {
        phase = (simTimer - 7.0f) / 3.0f;
        currentSky = LerpColor(skySenja, skyMalam, phase); // Berubah ke malam
    } else {
        currentSky = skyMalam; // Diam di malam hari
    }

    // Gambar Latar Belakang (Langit)
    ClearBackground(currentSky);

    // 3. GAMBAR TANAH (Horizon)
    // Tanah kita ada di Y = -1.0 pada sistem kartesian
    int groundY = G_OriginY - (int)(-1.0f * G_TickStep);
    
    // Tarik garis horizontal untuk mewarnai tanah dari horizon ke bawah layar
    for (int y = groundY; y <= SCREEN_H; y++) {
        // Semakin malam, tanah juga ikut menggelap
        Color groundCol = LerpColor((Color){50, 100, 40, 255}, (Color){15, 25, 15, 255}, (simTimer/10.0f));
        BresenhamLine(0, y, SCREEN_W, y, groundCol);
    }
    Bres_ThickLine(0, groundY, SCREEN_W, groundY, 4, DARKBROWN); // Garis batas tanah

    // 4. RENDER OBJEK (Nanti diletakkan di sini)
    // DrawComplexTent(...);
    // DrawComplexTree(...);
    // if (simTimer > 8.0f) DrawComplexCampfire(..., animTime); // Api nyala malam

    // 5. UI & DEBUGGING
    if (GuiButton(btnBack)) {
        *currentScreen = SCREEN_MENU;
        simTimer = 0.0f; // Reset waktu jika keluar
    }

    // Teks Indikator Waktu
    char timeTxt[30]; snprintf(timeTxt, 30, "Waktu Simulasi: %.1f s", simTimer);
    DrawText(timeTxt, SCREEN_W - 220, 20, 20, RAYWHITE);
}