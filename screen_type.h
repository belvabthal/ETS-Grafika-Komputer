#ifndef SCREEN_TYPE_H
#define SCREEN_TYPE_H

#include <raylib.h>

#define SCREEN_W 1280
#define SCREEN_H 720

#define colGermanDark (Color){45, 48, 40, 255}   
#define colFlatGreen  (Color){59, 107, 53, 255} 
#define colLizard     (Color){115, 145, 78, 255} 
#define colAcorn      (Color){120, 110, 35, 255} 

#define colBrightGold (Color){255, 195, 0, 255}  
#define colWhiteText  (Color){250, 250, 250, 255}

typedef enum { 
    SCREEN_MENU = 0, 
    SCREEN_SIMULATION, 
    SCREEN_OBJECT_VIEWER, 
    SCREEN_ABOUT,
    SCREEN_GUIDE
} AppScreen;

#endif