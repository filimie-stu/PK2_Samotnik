#pragma once
#include "IScore.h"
typedef struct score
{
    IScore* iScore;
    int takedowns;
    int goal;
} Score;

Score* Score_new();
IScore* Score_asIScore(Score* self);
Observable* Score_asObservable(Score* self);
void Score_destroy(Score* self);
void Score_increment(Score* self);
void Score_decrement(Score *self);
int Score_getPoints(Score* self);
int Score_getGoal(Score* self);