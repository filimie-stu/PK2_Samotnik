#pragma once
#include <stdlib.h>
#include "Vector2D.h"
#include "Field.h"
#include "Observable.h"

typedef enum
{
    NO_ACTION, JUMP, ACTIVATE_TOKEN
} ClickResult;

typedef struct board 
{
    Observable* observable;
    Vector2D dimensions;
    Field** fields;
    Field* activeField;

    int tokenCount;
} Board;

Board* Board_newFromFile(const char* relativePath);
void Board_destroy(Board* self);

ClickResult Board_clickField(Board* self, Vector2D coords);