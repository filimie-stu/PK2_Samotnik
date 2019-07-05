#include "IScore.h"
#include <stdlib.h>
#include <stdio.h>


/**
 * \brief Interfejs implementowany przez model wyniku (Score).
 * \sa score
*/
typedef struct i_score
{
    void *implementationObject;
    int (*getPointsOverride)(void *implObject);
    int (*getGoalOverride)(void *implObject);
    void (*incrementOverride)(void *implObject);
    void (*decrementOverride)(void *implObject);
    int (*hasWonOverride)(void* implObject);
    void (*destroyOverride)(void* implObject);
    Observable *observable;
} IScore;

IScore *IScore_new(
    void *implObject,
    int (*getPointsOverride)(void *implObject),
    int (*getGoalOverride)(void *implObject),
    void (*incrementOverride)(void *implObject),
    void (*decrementOverride)(void *implObject),
    int(*hasWonOverride)(void* implObject),
    void (*destroyOverride)(void* implObject))
{
    if (!implObject || !getPointsOverride || !getGoalOverride || !incrementOverride || !decrementOverride || !hasWonOverride || !destroyOverride)
    {
        printf("Error: NULL passed as interface override.\n");
    }
    IScore *created = (IScore *)malloc(sizeof(IScore));
    created->implementationObject = implObject;
    created->getGoalOverride = getGoalOverride;
    created->getPointsOverride = getPointsOverride;
    created->incrementOverride = incrementOverride;
    created->decrementOverride = decrementOverride;
    created->hasWonOverride = hasWonOverride;
    created->destroyOverride = destroyOverride;
    created->observable = Observable_new(created);

    return created;
}

Observable *IScore_asObservable(IScore *self)
{
    return self->observable;
}

void IScore_destroy(IScore *self, int destroyDerivedTypes)
{
    if (destroyDerivedTypes)
    {
        self->destroyOverride(self->implementationObject);
    }
    else
    {
        Observable_destroy(self->observable);
        free(self);
    }
    
}

int IScore_getPoints(IScore *self)
{
    return self->getPointsOverride(self->implementationObject);
}

int IScore_getGoal(IScore *self)
{
    return self->getGoalOverride(self->implementationObject);
}

void IScore_increment(IScore *self)
{
    self->incrementOverride(self->implementationObject);
}
void IScore_decrement(IScore *self)
{
    self->decrementOverride(self->implementationObject);
}
int IScore_hasWon(IScore* self)
{
    self->hasWonOverride(self->implementationObject);
}