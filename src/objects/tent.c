#include "tent.h"
#include "coords.h"
#include "src/algo/bresenham.h" 
#include <math.h> 
#include <stdio.h>

void DrawComplexTent(float cx, float cy, float scale, bool isOutlineMode) {
    float width = 6.0f * scale;  
    float height = 4.0f * scale; 
    float doorWidth = 2.0f * scale;
    float doorHeight = 2.5f * scale;

    float x_t0 = cx - (width / 2); 
    float y_t0 = cy;
    float x_t1 = cx + (width / 2); 
    float y_t1 = cy;
    float x_t2 = cx;               
    float y_t2 = cy + height;

    float x_d0 = cx - (doorWidth / 2);
    float y_d0 = cy;
    float x_d1 = cx + (doorWidth / 2);
    float y_d1 = cy;
    float x_d2 = cx;
    float y_d2 = cy + doorHeight;

    float x_ropeLeft = cx - (width / 2) - (1.5f * scale);
    float y_ropeLeft = cy;
    float x_ropeRight = cx + (width / 2) + (1.5f * scale);
    float y_ropeRight = cy;

    if (isOutlineMode) {
        Color lineCol = RAYWHITE;
        BresenhamLine(MAP_X(x_t0), MAP_Y(y_t0), MAP_X(x_t1), MAP_Y(y_t1), lineCol);
        BresenhamLine(MAP_X(x_t1), MAP_Y(y_t1), MAP_X(x_t2), MAP_Y(y_t2), lineCol);
        BresenhamLine(MAP_X(x_t2), MAP_Y(y_t2), MAP_X(x_t0), MAP_Y(y_t0), lineCol);

        BresenhamLine(MAP_X(x_d0), MAP_Y(y_d0), MAP_X(x_d1), MAP_Y(y_d1), lineCol);
        BresenhamLine(MAP_X(x_d1), MAP_Y(y_d1), MAP_X(x_d2), MAP_Y(y_d2), lineCol);
        BresenhamLine(MAP_X(x_d2), MAP_Y(y_d2), MAP_X(x_d0), MAP_Y(y_d0), lineCol);

        BresenhamLine(MAP_X(x_t2), MAP_Y(y_t2), MAP_X(x_ropeLeft), MAP_Y(y_ropeLeft), DARKGRAY);
        BresenhamLine(MAP_X(x_t2), MAP_Y(y_t2), MAP_X(x_ropeRight), MAP_Y(y_ropeRight), DARKGRAY);

        BresenhamLine(MAP_X(x_ropeLeft), MAP_Y(y_ropeLeft), MAP_X(x_ropeLeft) - 5, MAP_Y(y_ropeLeft - 0.5f) + 10, RED);
        BresenhamLine(MAP_X(x_ropeRight), MAP_Y(y_ropeRight), MAP_X(x_ropeRight) + 5, MAP_Y(y_ropeRight - 0.5f) + 10, RED);
    } else {
        int sy_top = MAP_Y(y_t2);
        int sy_bottom = MAP_Y(y_t0);
        int sx_top = MAP_X(x_t2);
        int sx_left = MAP_X(x_t0);
        int sx_right = MAP_X(x_t1);

        int deltaY = sy_bottom - sy_top;
        if (deltaY > 0) {
            for (int y = sy_top; y <= sy_bottom; y++) {
                float t = (float)(y - sy_top) / (float)deltaY;
                int cX_left = sx_top + (int)(t * (sx_left - sx_top));
                int cX_right = sx_top + (int)(t * (sx_right - sx_top));
                FastHorizontalLine(cX_left, cX_right, y, ORANGE);
            }
        }

        int sdy_top = MAP_Y(y_d2);
        int sdy_bottom = MAP_Y(y_d0);
        int sdx_top = MAP_X(x_d2);
        int sdx_left = MAP_X(x_d0);
        int sdx_right = MAP_X(x_d1);

        int d_deltaY = sdy_bottom - sdy_top;
        if (d_deltaY > 0) {
            for (int y = sdy_top; y <= sdy_bottom; y++) {
                float t = (float)(y - sdy_top) / (float)d_deltaY;
                int cX_left = sdx_top + (int)(t * (sdx_left - sdx_top));
                int cX_right = sdx_top + (int)(t * (sdx_right - sdx_top));
                FastHorizontalLine(cX_left, cX_right, y, DARKBROWN);
            }
        }
        
        BresenhamLine(MAP_X(x_t2), MAP_Y(y_t2), MAP_X(x_ropeLeft), MAP_Y(y_ropeLeft), DARKGRAY);
        BresenhamLine(MAP_X(x_t2), MAP_Y(y_t2), MAP_X(x_ropeRight), MAP_Y(y_ropeRight), DARKGRAY);
        
        int pegLeftX = MAP_X(x_ropeLeft);
        int pegLeftY = MAP_Y(y_ropeLeft);
        int pegRightX = MAP_X(x_ropeRight);
        int pegRightY = MAP_Y(y_ropeRight);
        
        Bres_ThickLine(pegLeftX, pegLeftY, pegLeftX - 6, pegLeftY + 12, 3, LIGHTGRAY); 
        Bres_ThickLine(pegLeftX + 1, pegLeftY - 3, pegLeftX - 2, pegLeftY + 2, 4, RED); 
        Bres_ThickLine(pegRightX, pegRightY, pegRightX + 6, pegRightY + 12, 3, LIGHTGRAY); 
        Bres_ThickLine(pegRightX - 1, pegRightY - 3, pegRightX + 2, pegRightY + 2, 4, RED); 
    }
}