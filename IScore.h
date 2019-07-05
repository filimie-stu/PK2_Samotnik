#pragma once
#include "Observable.h"



typedef struct i_score IScore;

/** \memberof i_score */
IScore *IScore_new(
    void *implObject,
    int (*getPointsOverride)(void *implObject),
    int (*getGoalOverride)(void *implObject),
    void (*incrementOverride)(void* implObject),
    void (*decrementOverride)(void* implObject),
    int (*hasWonOverride)(void* implObject),
    void (*destroyOverride)(void* implObject));

/** \memberof i_score */
Observable *IScore_asObservable(IScore *self);

/** \memberof i_score */
void IScore_destroy(IScore *self, int destroyDerivedTypes);

/** \memberof i_score */
void IScore_increment(IScore* self);

/** \memberof i_score */
void IScore_decrement(IScore* self);

/** \memberof i_score */
int IScore_getPoints(IScore *self);

/** \memberof i_score */
int IScore_getGoal(IScore *self);

/** \memberof i_score */
int IScore_hasWon(IScore* self);