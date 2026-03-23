#include <raylib.h>
#include "screen_type.h" // 1. MENGGUNAKAN HEADER BARU
#include "src/ui/button.h"
#include "src/screens/screen_patterns.h"
#include "coords.h" 

int main(void) {
    // 2. MENGGUNAKAN MAKRO UKURAN LAYAR DARI DOSEN
    InitWindow(SCREEN_W, SCREEN_H, "Simulasi Camping - Grafika Komputer");
    SetTargetFPS(60);

    // 3. RACIKAN KOORDINAT DOSEN
    G_OriginX  = SCREEN_W / 2;
    G_OriginY  = SCREEN_H / 2 + 30; // Digeser ke bawah untuk ruang Header
    G_TickStep = 38;                // Skala grid presisi

    AppScreen currentScreen = SCREEN_MENU;

    // 4. POSISI TOMBOL DISESUAIKAN KE TENGAH LAYAR (X = 400)
    UIButton btnSim =   {{400, 250, 200, 50}, "Animasi 2D", DARKGRAY, GRAY, WHITE};
    UIButton btnObj =   {{400, 320, 200, 50}, "Pola 2D", DARKGRAY, GRAY, WHITE};
    UIButton btnAbout = {{400, 390, 200, 50}, "About", DARKGRAY, GRAY, WHITE};
    UIButton btnExit =  {{400, 460, 200, 50}, "Exit", MAROON, RED, WHITE};
    
    UIButton btnBack =  {{20, 20, 100, 40}, "< Back", DARKGRAY, GRAY, WHITE};

    while (!WindowShouldClose()) {
        BeginDrawing();

        switch (currentScreen) {
            case SCREEN_MENU:
                ClearBackground(RAYWHITE);
                // Judul juga digeser ke tengah
                DrawText("SIMULASI CAMPING MATEMATIS", 270, 100, 28, DARKBLUE);
                
                if (GuiButton(btnSim)) currentScreen = SCREEN_SIMULATION;
                if (GuiButton(btnObj)) currentScreen = SCREEN_OBJECT_VIEWER;
                if (GuiButton(btnAbout)) currentScreen = SCREEN_ABOUT;
                if (GuiButton(btnExit)) goto end_loop; 
                break;

            case SCREEN_SIMULATION:
                ClearBackground(SKYBLUE); 
                DrawText("Layar Simulasi (Animasi di sini)", 350, 300, 20, BLACK);
                if (GuiButton(btnBack)) currentScreen = SCREEN_MENU;
                break;

            case SCREEN_OBJECT_VIEWER:
                DrawBlueprintPatternsScreen(btnBack, (int*)&currentScreen);
                break;

            case SCREEN_ABOUT:
                ClearBackground(LIGHTGRAY);
                DrawText("Tugas Praktikum Grafika Komputer", 330, 300, 20, BLACK);
                if (GuiButton(btnBack)) currentScreen = SCREEN_MENU;
                break;
        }

        EndDrawing();
    }

end_loop:
    CloseWindow();
    return 0;
}