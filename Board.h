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
    Vector2D dimensions;
    Field** fields;
    Field* activeField;

    int tokenCount;
} Board;

Board* Board_newFromFile(const char* relativePath);
void Board_destroy(Board* self);
IBoard* Board_asIBoard(Board* self);
Observable* Board_asObservable(Board* self);
int Board_tryJump(Board* self, Vector2D from, Vector2D to, JumpInfo* out_jumpData);
int Board_tryActivate(Board* self, Vector2D at);
void Board_rollbackJump(Board* self, JumpInfo jumpData);
Vector2D Board_getDimensions(Board* self);
FieldType Board_getFieldAt(Board* self, Vector2D at);
int Board_countTokens(Board* self);