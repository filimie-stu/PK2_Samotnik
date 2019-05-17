#include "ModelFactory.h"
#include "IModelFactory.h"
#include "Board.h"
#include "Score.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct model_factory
{
    IModelFactory *iModelFactory;
    const char *settingsFileRelativePath;
} ModelFactory;

static IBoard *private_wrapper_createBoard(void *vSelf, const char *relativeFilename);
static IScore *private_wrapper_createScore(void *vSelf, int goal, int handicap);
static IJumpHistory *private_wrapper_createJumpHistory(void *vSelf);

static IBoard *private_wrapper_createBoard(void *vSelf, const char *relativeFilename)
{
    return ModelFactory_createBoard((ModelFactory *)vSelf, relativeFilename);
}
static IScore *private_wrapper_createScore(void *vSelf, int goal, int handicap)
{
    return ModelFactory_createScore((ModelFactory *)vSelf, goal, handicap);
}
static IJumpHistory *private_wrapper_createJumpHistory(void *vSelf)
{
    return ModelFactory_createJumpHistory((ModelFactory *)vSelf);
}

IModelFactory *ModelFactory_asIModelFactory(ModelFactory *self)
{
    return self->iModelFactory;
}
ModelFactory *ModelFactory_new(const char *settingsFileRelativePath)
{
    ModelFactory *created = (ModelFactory *)malloc(sizeof(ModelFactory));
    if (!settingsFileRelativePath)
    {
        printf("Error: empty string passed as file path.\n");
    }
    created->iModelFactory = IModelFactory_new(
        created,
        private_wrapper_createBoard,
        private_wrapper_createScore,
        private_wrapper_createJumpHistory);
    created->settingsFileRelativePath = settingsFileRelativePath;
    return created;
}
void ModelFactory_destroy(ModelFactory *self)
{
    free(self);
}
IBoard *ModelFactory_createBoard(ModelFactory *self, const char *relativeFilename)
{
    return Board_asIBoard(Board_newFromFile(self->settingsFileRelativePath));
}
IScore *ModelFactory_createScore(ModelFactory *self, int goal, int handicap)
{
    return Score_asIScore(Score_new(goal, handicap));
}

IJumpHistory *ModelFactory_createJumpHistory(ModelFactory *self)
{
    return JumpHistory_asIJumpHistory(JumpHistory_new());
}
