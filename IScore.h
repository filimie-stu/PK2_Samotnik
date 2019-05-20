#pragma once
#include "Observable.h"



typedef struct i_score IScore;

IScore *IScore_new(
    void *implObject,
    int (*getPointsOverride)(void *implObject),
    int (*getGoalOverride)(void *implObject),
    void (*incrementOverride)(void* implObject),
    void (*decrementOverride)(void* implObject),
    int (*hasWonOverride)(void* implObject),
    void (*destroyOverride)(void* implObject));
Observable *IScore_asObservable(IScore *self);
void IScore_destroy(IScore *self, int destroyDerivedTypes);
void IScore_increment(IScore* self);
void IScore_decrement(IScore* self);
int IScore_getPoints(IScore *self);
int IScore_getGoal(IScore *self);
int IScore_hasWon(IScore* self);