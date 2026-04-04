#include "screen_about.h"
#include "screen_type.h"

void DrawAboutScreen(UIButton btnBack, int* currentScreen) {
    ClearBackground(colGermanDark);
    
    DrawRectangle(SCREEN_W/2 - 300, 120, 600, 480, Fade(BLACK, 0.4f));
    DrawRectangleLinesEx((Rectangle){SCREEN_W/2 - 300, 120, 600, 480}, 2, colLizard);
    
    const char* aboutTitle = "About";
    DrawText(aboutTitle, (SCREEN_W/2) - (MeasureText(aboutTitle, 28)/2), 160, 28, colBrightGold);
    DrawLine(SCREEN_W/2 - 250, 200, SCREEN_W/2 + 250, 200, colLizard);
    
    DrawText("NAMA:", SCREEN_W/2 - 250, 240, 20, colBrightGold);
    DrawText("Belva Abthal Hidayat", SCREEN_W/2 - 100, 240, 22, colWhiteText);
    
    DrawText("NIM:", SCREEN_W/2 - 250, 290, 20, colBrightGold);
    DrawText("241524035", SCREEN_W/2 - 100, 290, 22, colWhiteText);
    
    DrawText("KELAS:", SCREEN_W/2 - 250, 340, 20, colBrightGold);
    DrawText("2B - D4", SCREEN_W/2 - 100, 340, 22, colWhiteText);

    if (GuiButton(btnBack)) *currentScreen = SCREEN_MENU;
}