#pragma once
#include "IModelFactory.h"

typedef struct model_factory ModelFactory;


ModelFactory* ModelFactory_new(const char* settingsFileRelativePath);
IModelFactory* ModelFactory_asIModelFactory(ModelFactory* self);
void ModelFactory_destroy(ModelFactory* self);
IBoard* ModelFactory_createBoard(ModelFactory* self);
IScore* ModelFactory_createScore(ModelFactory* self);
IJumpHistory* ModelFactory_createJumpHistory(ModelFactory* self);