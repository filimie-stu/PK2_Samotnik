#pragma once
#include "IModelFactory.h"

typedef struct model_factory ModelFactory;

/** \memberof model_factory */
ModelFactory* ModelFactory_new();

/** \memberof model_factory */
IModelFactory* ModelFactory_asIModelFactory(ModelFactory* self);

/** \memberof model_factory */
void ModelFactory_destroy(ModelFactory* self);

/** \memberof model_factory */
IBoard* ModelFactory_createBoard(ModelFactory* self, const char* relativeFilename);

/** \memberof model_factory */
IScore* ModelFactory_createScore(ModelFactory* self, int goal, int handicap);

/** \memberof model_factory */
IJumpHistory* ModelFactory_createJumpHistory(ModelFactory* self);