#pragma once
#include "Vector2D.h"
#include "JumpInfo.h"
#include "Observable.h"
typedef struct i_board IBoard;

IBoard* IBoard_new(
    void* implObject,
    void(*destroyOverride)(void*implObject),
    int(*tryJumpOverride)(void* implObject, Vector2D from, Vector2D to),
    int(*tryActivateOverride)(void* implObject, Vector2D at),
    void(*rollbackJumpOverride)(void* implObjet, JumpInfo jumpData)
);
void IBoard_destroy(IBoard* self, int destroyDerivedTypes);
int IBoard_tryJump(IBoard* self, Vector2D from, Vector2D to);
int IBoard_tryActivate(IBoard* self, Vector2D at);
void IBoard_rollbackJump(IBoard* self, JumpInfo jumpData);
Observable* IBoard_asObservable(IBoard* self);