#pragma once

typedef struct vector_2d 
{
    int x;
    int y;
} Vector2D;


Vector2D Vector2D_create(int x, int y);
Vector2D Vector2D_calculateMidpoint(Vector2D first, Vector2D second);
int Vector2D_equals(Vector2D a, Vector2D b);