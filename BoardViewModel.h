#pragma once
#include "Vector2D.h"
#include "Observable.h"
#include "Common.h"

#define MAX_DIM_X 100
#define MAX_DIM_Y 100

typedef struct board_view_model
{
    Observable* boardObservable;
    Vector2D dimensions;
    FieldType fields[MAX_DIM_X][MAX_DIM_Y];

} BoardViewModel;

