#include <raylib.h>
#include "src/ui/button.h"

typedef enum {
    SCREEN_MENU = 0,
    SCREEN_SIMULATION,
    SCREEN_OBJECT_VIEWER,
    SCREEN_ABOUT
} AppScreen;

int main(void) {
    const int screenW = 800;
    const int screenH = 600;

    InitWindow(screenW, screenH, "Simulasi Camping - Grafika Komputer");
    SetTargetFPS(60);

    AppScreen currentScreen = SCREEN_MENU;

    UIButton btnSim =   {{300, 200, 200, 50}, "Play Simulasi", DARKGRAY, GRAY, WHITE};
    UIButton btnObj =   {{300, 270, 200, 50}, "Objek 2D", DARKGRAY, GRAY, WHITE};
    UIButton btnAbout = {{300, 340, 200, 50}, "About", DARKGRAY, GRAY, WHITE};
    UIButton btnExit =  {{300, 410, 200, 50}, "Exit", MAROON, RED, WHITE};
    
    UIButton btnBack =  {{20, 20, 100, 40}, "< Back", DARKGRAY, GRAY, WHITE};

    while (!WindowShouldClose()) {
        BeginDrawing();

        switch (currentScreen) {
            case SCREEN_MENU:
                ClearBackground(RAYWHITE);
                DrawText("SIMULASI CAMPING MATEMATIS", 170, 80, 28, DARKBLUE);
                
                if (GuiButton(btnSim)) currentScreen = SCREEN_SIMULATION;
                if (GuiButton(btnObj)) currentScreen = SCREEN_OBJECT_VIEWER;
                if (GuiButton(btnAbout)) currentScreen = SCREEN_ABOUT;
                if (GuiButton(btnExit)) goto end_loop; 
                break;

            case SCREEN_SIMULATION:
                ClearBackground(SKYBLUE); 
                DrawText("Layar Simulasi (Animasi di sini)", 250, 250, 20, BLACK);
                if (GuiButton(btnBack)) currentScreen = SCREEN_MENU;
                break;

            case SCREEN_OBJECT_VIEWER:
                ClearBackground(BLACK);
                DrawText("Layar Objek 2D (Grid Koordinat di sini)", 200, 250, 20, GREEN);
                if (GuiButton(btnBack)) currentScreen = SCREEN_MENU;
                break;

            case SCREEN_ABOUT:
                ClearBackground(LIGHTGRAY);
                DrawText("Tugas Praktikum Grafika Komputer", 230, 250, 20, BLACK);
                if (GuiButton(btnBack)) currentScreen = SCREEN_MENU;
                break;
        }

        EndDrawing();
    }

end_loop:
    CloseWindow();
    return 0;
}