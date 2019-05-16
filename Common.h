#pragma once

typedef enum direction
{
    UP = 0,
    DOWN = 1,
    LEFT = 2,
    RIGHT = 3
} Direction;


typedef enum fieldType
{
    REGULAR_TOKEN,
    ACTIVE_TOKEN,
    EMPTY,
    FORBIDDEN

} FieldType;

FieldType FieldType_fromChar(char c);
char FieldType_toChar(FieldType ft);
