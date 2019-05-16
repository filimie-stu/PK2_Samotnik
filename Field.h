#pragma once
#include "Vector2D.h"
#include "Common.h"

typedef struct field
{
    Vector2D coords;
    FieldType contents;

} Field;

Field Field_create(int x, int y, FieldType contents);

