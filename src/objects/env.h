#ifndef ENV_H
#define ENV_H

#include "raylib.h"

void DrawComplexTree(float cx, float cy, float scale, bool isOutlineMode);
void DrawComplexCampfire(float cx, float cy, float scale, bool isOutlineMode, float animTime);
void DrawStarField(float minX, float maxX, float animTime, float alphaFade, bool isOutlineMode);

#endif