#pragma once
#include "Observable.h"
typedef struct score
{
    int takedowns;
    int goal;
    Observable* observable;
} Score;

Score* Score_new();
void Score_destroy(Score* self);
void Score_increment(Score* self);