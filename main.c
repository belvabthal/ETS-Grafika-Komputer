#include <raylib.h>
#include "screen_type.h" 
#include "src/ui/button.h"
#include "src/screens/screen_patterns.h"
#include "src/screens/screen_simulation.h"
#include "src/screens/screen_about.h"  
#include "src/screens/screen_guide.h"  
#include "coords.h" 

#include "src/algo/bresenham.h"
#include "src/objects/tent.h"
#include "src/objects/env.h"

int main(void) {
    InitWindow(SCREEN_W, SCREEN_H, "Simulasi Camping - Grafika Komputer");
    SetTargetFPS(60);

    G_OriginX  = SCREEN_W / 2;
    G_OriginY  = SCREEN_H / 2 + 50; 
    G_TickStep = 38;                

    AppScreen currentScreen = SCREEN_MENU;

    int btnWidth = 260;
    int btnHeight = 45; 
    int btnX = (SCREEN_W / 2) - (btnWidth / 2); 

    // Teks tombol sekarang memakai colWhiteText agar sangat kontras!
    UIButton btnSim   = {{btnX, 210, btnWidth, btnHeight}, "Animasi 2D", colFlatGreen, colLizard, colWhiteText};
    UIButton btnObj   = {{btnX, 270, btnWidth, btnHeight}, "Pola 2D", colFlatGreen, colLizard, colWhiteText};
    UIButton btnAbout = {{btnX, 330, btnWidth, btnHeight}, "About", colFlatGreen, colLizard, colWhiteText};
    UIButton btnGuide = {{btnX, 390, btnWidth, btnHeight}, "Guide", colFlatGreen, colLizard, colWhiteText};
    
    // Tombol Keluar (Teks putih)
    UIButton btnExit  = {{btnX, 450, btnWidth, btnHeight}, "Keluar", colAcorn, colLizard, colWhiteText};
    
    UIButton btnBack  = {{20, 20, 120, 40}, "< Kembali", colFlatGreen, colLizard, colWhiteText};

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_F11)) {
            ToggleFullscreen();
        }

        BeginDrawing();

        switch (currentScreen) {
            case SCREEN_MENU:
            {
                ClearBackground(colGermanDark); 
                
                int groundY = G_OriginY + G_TickStep; 
                DrawRectangle(0, groundY, SCREEN_W, SCREEN_H - groundY, (Color){30, 35, 25, 255});
                Bres_ThickLine(0, groundY, SCREEN_W, groundY, 4, (Color){20, 20, 15, 255});

                DrawComplexTree(-12.0f, -1.0f + 2.5f, 2.5f, false);
                DrawComplexTree( 12.0f, -1.0f + 2.0f, 2.0f, false);
                DrawComplexTree(-16.0f, -1.0f + 1.5f, 1.5f, false);
                DrawComplexTree( 15.0f, -1.0f + 1.8f, 1.8f, false);
                
                DrawComplexTent(-6.0f, -1.0f, 1.8f, false);

                int panelWidth = 400;
                int panelHeight = 460;
                int panelX = (SCREEN_W / 2) - (panelWidth / 2);
                DrawRectangle(panelX, 60, panelWidth, panelHeight, Fade(BLACK, 0.4f));
                DrawRectangleLinesEx((Rectangle){panelX, 60, panelWidth, panelHeight}, 2, colLizard);
                
                // Judul menyala dengan colBrightGold
                const char* title = "CAMPING SIMULATION";
                int titleWidth = MeasureText(title, 32);
                DrawText(title, (SCREEN_W / 2) - (titleWidth / 2), 100, 32, colBrightGold);

                // Subjudul menggunakan putih
                const char* subtitle = "Tugas Besar Grafika Komputer";
                int subWidth = MeasureText(subtitle, 16);
                DrawText(subtitle, (SCREEN_W / 2) - (subWidth / 2), 140, 16, colWhiteText);

                DrawLine((SCREEN_W / 2) - 150, 170, (SCREEN_W / 2) + 150, 170, colBrightGold);

                if (GuiButton(btnSim)) currentScreen = SCREEN_SIMULATION;
                if (GuiButton(btnObj)) currentScreen = SCREEN_OBJECT_VIEWER;
                if (GuiButton(btnAbout)) currentScreen = SCREEN_ABOUT;
                if (GuiButton(btnGuide)) currentScreen = SCREEN_GUIDE;
                if (GuiButton(btnExit)) goto end_loop; 
                break;
            }

            case SCREEN_SIMULATION:
                DrawSimulationScreen(btnBack, (int*)&currentScreen);
                break;
            case SCREEN_OBJECT_VIEWER:
                DrawBlueprintPatternsScreen(btnBack, (int*)&currentScreen);
                break;
            case SCREEN_ABOUT:
                DrawAboutScreen(btnBack, (int*)&currentScreen);
                break;
            case SCREEN_GUIDE:
                DrawGuideScreen(btnBack, (int*)&currentScreen);
                break;
        }

        EndDrawing();
    }

end_loop:
    CloseWindow();
    return 0;
}