#pragma once
#include "Colour.h"
#include "math.h"

void DrawBox(float x, float y, float width, float height, float thickness, float r, float g, float b, float a, bool filled);
void DrawBoxVec(Vector startingPos, float width, float height, float thickness, Colour colour, bool filled);

// Draw a rounded box on the screan 
void DrawRoundedBox(float x, float y, float x2, float y2, float radius_x, float radius_y, float thickness, float r, float g, float b, float a, bool filled);
