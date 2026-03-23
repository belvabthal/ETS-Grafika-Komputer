#ifndef SCREEN_TYPE_H
#define SCREEN_TYPE_H

// Resolusi layar standar dari dosen
#define SCREEN_W 1000
#define SCREEN_H 700

// Kita sesuaikan nama layarnya dengan kebutuhan tugas simulasi kita
typedef enum { 
    SCREEN_MENU = 0, 
    SCREEN_SIMULATION, 
    SCREEN_OBJECT_VIEWER, 
    SCREEN_ABOUT 
} AppScreen;

#endif