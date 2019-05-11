#pragma once
#include "Vector2D.h"

typedef enum fieldType
{
    REGULAR_TOKEN,
    ACTIVE_TOKEN,
    EMPTY,
    FORBIDDEN

} FieldType;

typedef struct field
{
    Vector2D coords;
    FieldType contents;

} Field;

Field Field_create(int x, int y, FieldType contents);
FieldType FieldType_fromChar(char c);
char FieldType_toChar(FieldType ft);
