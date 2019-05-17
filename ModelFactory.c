#include "ModelFactory.h"
#include "IModelFactory.h"
#include "Board.h"
#include <stdlib.h>
#include <stdio.h>


typedef struct model_factory 
{
    IModelFactory* iModelFactory;
    const char* settingsFileRelativePath;
} ModelFactory;

static IBoard* private_wrapper_createBoard(void* vSelf);
static IScore* private_wrapper_createScore(void * vSelf);
static IJumpHistory* private_wrapper_createJumpHistory(void* vSelf);


static IBoard* private_wrapper_createBoard(void* vSelf)
{
    return ModelFactory_createBoard((ModelFactory*)vSelf);
}
static IScore* private_wrapper_createScore(void * vSelf)
{
    return ModelFactory_createScore((ModelFactory*)vSelf);
}
static IJumpHistory* private_wrapper_createJumpHistory(void* vSelf)
{
    return ModelFactory_createJumpHistory((ModelFactory*)vSelf);
}

IModelFactory *ModelFactory_asIModelFactory(ModelFactory* self)
{
    return self->iModelFactory;
}
ModelFactory* ModelFactory_new(const char* settingsFileRelativePath)
{
    ModelFactory* created = (ModelFactory*)malloc(sizeof(ModelFactory));
    if (!settingsFileRelativePath)
    {
        printf("Error: empty string passed as file path.\n");
    }
    created->iModelFactory=IModelFactory_new(
        created,
        private_wrapper_createBoard,
        private_wrapper_createScore,
        private_wrapper_createJumpHistory);
    created->settingsFileRelativePath = settingsFileRelativePath;
    return created;
}
void ModelFactory_destroy(ModelFactory* self)
{
    free(self);
}
IBoard* ModelFactory_createBoard(ModelFactory* self)
{
    return Board_asIBoard(Board_newFromFile(self->settingsFileRelativePath));
}
IScore* ModelFactory_createScore(ModelFactory* self)
{
    //todo
    return NULL;
}
IJumpHistory* ModelFactory_createJumpHistory(ModelFactory* self)
{
    //todo
    return NULL;
}