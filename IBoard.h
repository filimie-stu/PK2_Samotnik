#pragma once
#include "Common.h"
#include "Vector2D.h"
#include "JumpInfo.h"
#include "Observable.h"
typedef struct i_board IBoard;


/** \memberof i_board */
IBoard* IBoard_new(
    void* implObject,
    void(*destroyOverride)(void*implObject),
    int(*tryJumpOverride)(void* implObject, Vector2D from, Vector2D to, JumpInfo* out_jumpData),
    int(*tryActivateOverride)(void* implObject, Vector2D at),
    void(*rollbackJumpOverride)(void* implObjet, JumpInfo jumpData),
    FieldType (*getFieldAtOverride)(void *implObject, Vector2D at),
    Vector2D (*getDimensionsOverride)(void* implObject),
    int(*countTokensOverride)(void* implObject),
    int (*isDeadEndOverride)(void* implObject)

);

/** \memberof i_board */
int IBoard_tryJump(IBoard* self, Vector2D from, Vector2D to, JumpInfo* out_jumpData );
/** \memberof i_board */
int IBoard_tryActivate(IBoard* self, Vector2D at);
/** \memberof i_board */
void IBoard_rollbackJump(IBoard* self, JumpInfo jumpData);
/** \memberof i_board */
FieldType IBoard_getFieldAt(IBoard* self, Vector2D at);
/** \memberof i_board */
Vector2D IBoard_getDimensions(IBoard* self);
/** \memberof i_board */
int IBoard_countTokens(IBoard* self);
/** \memberof i_board */
int IBoard_isDeadEnd(IBoard* self);
/** \memberof i_board */
Observable* IBoard_asObservable(IBoard* self);
/** \memberof i_board */
void IBoard_destroy(IBoard* self, int destroyDerivedTypes);
