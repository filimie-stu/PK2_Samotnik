#pragma once
#include "IBoard.h"
#include "Vector2D.h"
#include "Field.h"
#include "Observable.h"
#include "JumpInfo.h"
#include "JumpHistory.h"

typedef enum
{
    NO_ACTION, JUMP, ACTIVATE_TOKEN
} ClickResult;

typedef struct board 
{
    IBoard* iBoard;
    Observable* observable;
    Vector2D dimensions;
    Field** fields;
    Field* activeField;

    int tokenCount;
} Board;

Board* Board_newFromFile(const char* relativePath);
void Board_destroy(Board* self);
int Board_tryJump(Board* self, Vector2D from, Vector2D to);
int Board_tryActivate(Board* self, Vector2D at);
void Board_rollbackJump(Board* self, JumpInfo jumpData);
IBoard* Board_asIBoard(Board* self);