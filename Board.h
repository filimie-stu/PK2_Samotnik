#pragma once
#include <stdlib.h>
#include "Vector2D.h"
#include "Field.h"

typedef struct board 
{
    Vector2D dimensions;
    Field** fields;
    Field* activeField;

    int tokenCount;
} Board;

Board* Board_newFromFile(const char* relativePath);
void Board_destroy(Board* self);