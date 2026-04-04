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

static bool isBlueprintMode = false;
static int currentObjIndex = 0;      
const int TOTAL_OBJECTS = 6;         

void DrawBlueprintPatternsScreen(UIButton btnBack, int* currentScreen) {
    ClearBackground((Color){20, 22, 18, 255});

    DrawCartesianGrid(BresenhamLine);

    if (IsKeyPressed(KEY_O)) {
        isBlueprintMode = !isBlueprintMode;
    }

    const char* btnModeTxt = isBlueprintMode ? "OUTLINE : ON (O)" : "OUTLINE : OFF (O)";
    
    Color btnModeCol = isBlueprintMode ? colBrightGold : colFlatGreen;
    Color btnModeHov = isBlueprintMode ? colAcorn : colLizard;
    
    UIButton btnToggle = {{20, SCREEN_H - 60, 200, 40}, btnModeTxt, btnModeCol, btnModeHov, colWhiteText};
    UIButton btnPrev = {{20, (SCREEN_H / 2) - 25, 40, 50}, "<", colFlatGreen, colLizard, colWhiteText};
    UIButton btnNext = {{SCREEN_W - 60, (SCREEN_H / 2) - 25, 40, 50}, ">", colFlatGreen, colLizard, colWhiteText};

    if (GuiButton(btnToggle)) {
        isBlueprintMode = !isBlueprintMode; 
    }
    if (GuiButton(btnPrev)) {
        currentObjIndex--;
        if (currentObjIndex < 0) currentObjIndex = TOTAL_OBJECTS - 1; 
    }
    if (GuiButton(btnNext)) {
        currentObjIndex++;
        if (currentObjIndex >= TOTAL_OBJECTS) currentObjIndex = 0; 
    }

    if (GuiButton(btnBack)) {
        *currentScreen = SCREEN_MENU; 
    }

    const char* objTitle = "";

    switch (currentObjIndex) {
        case 0: objTitle = "OBJEK 1: STRUKTUR TENDA"; DrawComplexTent(0.0f, -2.0f, 2.0f, isBlueprintMode); break;
        case 1: objTitle = "OBJEK 2: POHON KEMAH"; DrawComplexTree(0.0f, -3.0f, 2.5f, isBlueprintMode); break;
        case 2: objTitle = "OBJEK 3: API UNGGUN"; DrawComplexCampfire(0.0f, -3.0f, 2.0f, isBlueprintMode, 0.0f); break;
        case 3: objTitle = "OBJEK 4: ANATOMI ORANG"; DrawComplexPerson(0.0f, -3.0f, 1.5f, isBlueprintMode, 0.0f); break;
        case 4: objTitle = "OBJEK 5: KENDARAAN (MOBIL)"; DrawComplexCar(0.0f, -1.0f, 1.5f, isBlueprintMode, 0.0f); break;
        case 5: objTitle = "OBJEK 6: BINTANG PROSEDURAL"; DrawStarField(-20.0f, 20.0f, (float)GetTime(), 1.0f, isBlueprintMode); break;
    }

    int textWidth = MeasureText(objTitle, 20);
    DrawText(objTitle, (SCREEN_W / 2) - (textWidth / 2), 30, 20, colBrightGold);
}