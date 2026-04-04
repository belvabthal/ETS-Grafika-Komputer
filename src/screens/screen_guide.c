#include "screen_guide.h"
#include "screen_type.h"

void DrawGuideScreen(UIButton btnBack, int* currentScreen) {
    ClearBackground(colGermanDark);
    
    DrawRectangle(SCREEN_W/2 - 350, 100, 700, 520, Fade(BLACK, 0.4f));
    DrawRectangleLinesEx((Rectangle){SCREEN_W/2 - 350, 100, 700, 520}, 2, colLizard);
    
    const char* guideTitle = "Guide";
    DrawText(guideTitle, (SCREEN_W/2) - (MeasureText(guideTitle, 28)/2), 130, 28, colBrightGold);
    DrawLine(SCREEN_W/2 - 300, 170, SCREEN_W/2 + 300, 170, colLizard);
    
    DrawText("1. MENU ANIMASI 2D (SIMULASI)", SCREEN_W/2 - 300, 200, 20, colBrightGold);
    DrawText("- Menampilkan animasi prosedural dengan sistem kamera dinamis.", SCREEN_W/2 - 280, 230, 18, colWhiteText);
    DrawText("- Gunakan Klik Kanan + Geser Mouse untuk memindahkan layar.", SCREEN_W/2 - 280, 260, 18, colWhiteText);
    DrawText("- Gunakan Scroll Mouse untuk Zoom-In / Zoom-Out area simulasi.", SCREEN_W/2 - 280, 290, 18, colWhiteText);
    DrawText("- Gunakan tombol << dan >> untuk mengatur kecepatan simulasi.", SCREEN_W/2 - 280, 320, 18, colWhiteText);

    DrawText("2. MENU POLA 2D (BLUEPRINT)", SCREEN_W/2 - 300, 370, 20, colBrightGold);
    DrawText("- Menampilkan kerangka matematika dasar pembentuk objek.", SCREEN_W/2 - 280, 400, 18, colWhiteText);
    DrawText("- Gunakan tombol '<' dan '>' untuk mengganti objek yang dilihat.", SCREEN_W/2 - 280, 430, 18, colWhiteText);
    DrawText("- Tekan tombol 'Outline' di bawah layar untuk", SCREEN_W/2 - 280, 460, 18, colWhiteText);
    DrawText("  melihat garis algoritma Bresenham dan Midpoint secara transparan.", SCREEN_W/2 - 280, 490, 18, colWhiteText);

    if (GuiButton(btnBack)) *currentScreen = SCREEN_MENU;
}