#pragma once
#include "IBoard.h"
#include "IScore.h"
#include "IJumpHistory.h"

typedef struct i_model_factory IModelFactory;

IModelFactory* IModelFactory_new(
    void* implObject,
    IBoard* (*createBoardOverride)(void* implObject),
    IScore* (*createScoreOverride)(void* implObject),
    IJumpHistory* (*createJumpHistoryOverride)(void* implObject));

void IModelFactory_destroy(IModelFactory* self);
IBoard* IModelFactory_createBoard(IModelFactory* self);
IScore* IModelFactory_createScore(IModelFactory* self);
IJumpHistory* IModelFactory_createJumpHistory(IModelFactory* self);