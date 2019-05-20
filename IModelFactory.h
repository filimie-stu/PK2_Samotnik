#pragma once
#include "IBoard.h"
#include "IScore.h"
#include "IJumpHistory.h"

typedef struct i_model_factory IModelFactory;

IModelFactory *IModelFactory_new(
    void *implObject,
    IBoard *(*createBoardOverride)(void *implObject, const char *relativeFilename),
    IScore *(*createScoreOverride)(void *implObject, int goal, int handicap),
    IJumpHistory *(*createJumpHistoryOverride)(void *implObject),
    void (*destroyOverride)(void *implObject));

void IModelFactory_destroy(IModelFactory *self, int destroyDerivedTypes);
IBoard *IModelFactory_createBoard(IModelFactory *self, const char *relativeFilename);
IScore *IModelFactory_createScore(IModelFactory *self, int goal, int handicap);
IJumpHistory *IModelFactory_createJumpHistory(IModelFactory *self);
void IModelFactory_destroy(IModelFactory *self, int destroyDerivedTypes);