#include "screen_patterns.h"
#include "coords.h"
#include "screen_type.h"
#include "src/algo/bresenham.h" 
#include "src/objects/env.h"    
#include "src/objects/tent.h"
#include "src/objects/person.h"
#include "src/objects/car.h"
#include "src/ui/cartesian.h"   
#include <stdio.h>

// --- STATE VARIABLES ---
static bool isBlueprintMode = false;
static int currentObjIndex = 0;      // Melacak objek mana yang sedang dilihat
const int TOTAL_OBJECTS = 5;         // Total objek saat ini (0: Tenda, 1: Pohon)

void DrawBlueprintPatternsScreen(UIButton btnBack, int* currentScreen) {
    ClearBackground((Color){15, 25, 18, 255});

    // Panggil grid dosen
    DrawCartesianGrid(BresenhamLine);

    // --- INISIALISASI TOMBOL UI ---
    // Tombol Toggle Mode
    const char* btnModeTxt = isBlueprintMode ? "MODE: BLUEPRINT (AKTIF)" : "MODE: NORMAL (NONAKTIF)";
    UIButton btnToggle = {{20, SCREEN_H - 60, 300, 40}, btnModeTxt, MAROON, RED, WHITE};
    
    // Tombol Navigasi Kiri & Kanan (Diposisikan persis di tengah vertikal layar)
    UIButton btnPrev = {{20, (SCREEN_H / 2) - 25, 40, 50}, "<", DARKGRAY, GRAY, WHITE};
    UIButton btnNext = {{SCREEN_W - 60, (SCREEN_H / 2) - 25, 40, 50}, ">", DARKGRAY, GRAY, WHITE};

    // --- LOGIKA IMGUI & INTERAKSI ---
    if (GuiButton(btnToggle)) {
        isBlueprintMode = !isBlueprintMode; 
    }
    
    // Logika Navigasi (Melingkar / Carousel)
    if (GuiButton(btnPrev)) {
        currentObjIndex--;
        // Jika mundur dari 0, kembali ke objek terakhir
        if (currentObjIndex < 0) currentObjIndex = TOTAL_OBJECTS - 1; 
    }
    
    if (GuiButton(btnNext)) {
        currentObjIndex++;
        // Jika maju melebihi batas, kembali ke 0
        if (currentObjIndex >= TOTAL_OBJECTS) currentObjIndex = 0; 
    }

    if (GuiButton(btnBack)) {
        *currentScreen = SCREEN_MENU; 
    }

    // --- RENDER OBJEK BERDASARKAN INDEKS ---
    const char* objTitle = "";

    // Kita letakkan objek persis di tengah sumbu X (0.0f) agar terlihat heroik
    switch (currentObjIndex) {
        case 0:
            objTitle = "OBJEK 1: STRUKTUR TENDA";
            DrawComplexTent(0.0f, -2.0f, 2.0f, isBlueprintMode); 
            break;
        case 1:
            objTitle = "OBJEK 2: POHON KEMAH";
            DrawComplexTree(0.0f, -3.0f, 2.5f, isBlueprintMode);  
            break;
        case 2:
            objTitle = "OBJEK 3: API UNGGUN";
            DrawComplexCampfire(0.0f, -3.0f, 2.0f, isBlueprintMode);
            break;
        case 3:
            objTitle = "OBJEK 4: ANATOMI ORANG";
            DrawComplexPerson(0.0f, -3.0f, 1.5f, isBlueprintMode);
            break;
        case 4:
            objTitle = "OBJEK 5: KENDARAAN (MOBIL)";
            DrawComplexCar(0.0f, -1.0f, 1.5f, isBlueprintMode);
            break;
    }

    // --- RENDER INFORMASI TEKS ---
    // Gambar Judul Objek di atas grid
    int textWidth = MeasureText(objTitle, 20);
    DrawText(objTitle, (SCREEN_W / 2) - (textWidth / 2), 30, 20, YELLOW);
}