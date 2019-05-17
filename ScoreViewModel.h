#pragma once
#include "Observable.h"
typedef struct score_view_model
{
    int points;
    int goal;
    Observable* scoreObservable;
} ScoreViewModel;