#include "IModelFactory.h"
#include <stdlib.h>
#include <stdio.h>


/**
 * \brief Interfejs implementowany przez fabrykę modeli (ModelFactory).
 * \sa model_factory
*/
typedef struct i_model_factory
{
    void *implementationObject;
    IBoard *(*createBoardOverride)(void *implObject, const char *relativeFilename);
    IScore *(*createScoreOverride)(void *implObject, int goal, int handicap);
    IJumpHistory *(*createJumpHistoryOverride)(void *implObject);
    void (*destroyOverride)(void *implObject);
} IModelFactory;

IModelFactory *IModelFactory_new(
    void *implObject,
    IBoard *(*createBoardOverride)(void *implObject, const char *relativeFilename),
    IScore *(*createScoreOverride)(void *implObject, int goal, int handicap),
    IJumpHistory *(*createJumpHistoryOverride)(void *implObject),
    void (*destroyOverride)(void *implObject))
{
    if (!implObject || !createBoardOverride || !createScoreOverride || !createJumpHistoryOverride || !destroyOverride)
    {
        printf("Error: NULL passed as interface override.\n");
    }

    IModelFactory *created = (IModelFactory *)malloc(sizeof(IModelFactory));
    created->implementationObject = implObject;
    created->createBoardOverride = createBoardOverride;
    created->createScoreOverride = createScoreOverride;
    created->createJumpHistoryOverride = createJumpHistoryOverride;
    created->destroyOverride = destroyOverride;
    return created;
}

void IModelFactory_destroy(IModelFactory *self, int destroyDerivedTypes)
{
    if (destroyDerivedTypes)
    {
        self->destroyOverride(self->implementationObject);
    }
    else
    {
        free(self);
    }
}
IBoard *IModelFactory_createBoard(IModelFactory *self, const char *relativeFilename)
{
    return self->createBoardOverride(self->implementationObject, relativeFilename);
}
IScore *IModelFactory_createScore(IModelFactory *self, int goal, int handicap)
{
    return self->createScoreOverride(self->implementationObject, goal, handicap);
}
IJumpHistory *IModelFactory_createJumpHistory(IModelFactory *self)
{
    return self->createJumpHistoryOverride(self->implementationObject);
}