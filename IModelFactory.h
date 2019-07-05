#pragma once
#include "IBoard.h"
#include "IScore.h"
#include "IJumpHistory.h"

typedef struct i_model_factory IModelFactory;

/** \memberof i_model_factory */
IModelFactory *IModelFactory_new(
    void *implObject,
    IBoard *(*createBoardOverride)(void *implObject, const char *relativeFilename),
    IScore *(*createScoreOverride)(void *implObject, int goal, int handicap),
    IJumpHistory *(*createJumpHistoryOverride)(void *implObject),
    void (*destroyOverride)(void *implObject));

/** \memberof i_model_factory */
void IModelFactory_destroy(IModelFactory *self, int destroyDerivedTypes);

/** \memberof i_model_factory */
IBoard *IModelFactory_createBoard(IModelFactory *self, const char *relativeFilename);

/** \memberof i_model_factory */
IScore *IModelFactory_createScore(IModelFactory *self, int goal, int handicap);

/** \memberof i_model_factory */
IJumpHistory *IModelFactory_createJumpHistory(IModelFactory *self);

/** \memberof i_model_factory */
void IModelFactory_destroy(IModelFactory *self, int destroyDerivedTypes);