#pragma once
#include "IScore.h"
typedef struct score
{
    IScore* iScore;
    int takedowns;
    int goal;
    int handicap;
} Score;

Score* Score_new(int goal, int handicap);
IScore* Score_asIScore(Score* self);
Observable* Score_asObservable(Score* self);
void Score_destroy(Score* self);
void Score_increment(Score* self);
void Score_decrement(Score *self);
int Score_getPoints(Score* self);
int Score_getGoal(Score* self);
int Score_hasWon(Score* self);