#include "IModelFactory.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct i_model_factory 
{
    void* implementationObject;
    IBoard* (*createBoardOverride)(void* implObject);
    IScore* (*createScoreOverride)(void* implObject);
    IJumpHistory* (*createJumpHistoryOverride)(void* implObject);
} IModelFactory;


IModelFactory* IModelFactory_new(
    void* implObject,
    IBoard* (*createBoardOverride)(void* implObject),
    IScore* (*createScoreOverride)(void* implObject),
    IJumpHistory* (*createJumpHistoryOverride)(void* implObject))
{
    if (!implObject || !createBoardOverride || !createScoreOverride || !createJumpHistoryOverride)
    {
        printf("Error: NULL passed as interface override.\n");
    }

    IModelFactory* created = (IModelFactory*)malloc(sizeof(IModelFactory));
    created->createBoardOverride = createBoardOverride;
    created->createScoreOverride= createScoreOverride;
    created->createJumpHistoryOverride = createJumpHistoryOverride  ;

    return created;
}

void IModelFactory_destroy(IModelFactory* self)
{
    // todo
}
IBoard* IModelFactory_createBoard(IModelFactory* self)
{
    self->createBoardOverride(self->implementationObject);
}
IScore* IModelFactory_createScore(IModelFactory* self)
{
    self->createScoreOverride(self->implementationObject);
}
IJumpHistory* IModelFactory_createJumpHistory(IModelFactory* self)
{
    self->createJumpHistoryOverride(self->implementationObject);
}