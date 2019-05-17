#include "Score.h"
#include "IScore.h"
#include "SyncScoreArgs.h"
#include <stdlib.h>

static int private_wrapper_getPoints(void* vSelf);
static int private_wrapper_getGoal(void* vSelf);
static void private_wrapper_increment(void* vSelf);
static void private_wrapper_decrement(void* vSelf);

void private_wrapper_increment(void* vSelf)
{
    Score_increment((Score*)vSelf);
}
void private_wrapper_decrement(void* vSelf)
{
    Score_decrement((Score*)vSelf);
}

int private_wrapper_getPoints(void* vSelf)
{
    return Score_getPoints((Score*)vSelf);
}
int private_wrapper_getGoal(void* vSelf)
{
    return Score_getGoal((Score*)vSelf);
}

Score *Score_new()

{
    Score *created = (Score *)malloc(sizeof(Score));
    created->iScore = IScore_new(
        created,
        private_wrapper_getPoints,
        private_wrapper_getGoal,
        private_wrapper_increment,
        private_wrapper_decrement
    );
    created->goal = 10;
    created->takedowns = 0;
    return created;
}
void Score_destroy(Score *self)
{
    free(self);
}

int Score_getPoints(Score* self)
{
    return self->takedowns;
}
int Score_getGoal(Score* self)
{
    return self->goal;
}

int private_goalAchieved(Score *self)
{
    return self->takedowns >= self->goal;
}

IScore* Score_asIScore(Score* self)
{
    return self->iScore;
}
Observable* Score_asObservable(Score* self)
{
    return IScore_asObservable(self->iScore);
}
void Score_increment(Score *self)
{
    self->takedowns++;
    SyncScoreArgs args = {self->takedowns, self->goal - self->takedowns};
    Observable_notifyObservers(Score_asObservable(self), "sync_score", &args);

    if (private_goalAchieved(self))
    {
        Observable_notifyObservers(Score_asObservable(self), "goal_achieved", &args);
    }
}

void Score_decrement(Score *self)
{
    if (self->takedowns <= 0 )
    {
        printf("Error: cannot decrement score below 0!\n");
        return;
    }
    self->takedowns--;
    SyncScoreArgs args = {self->takedowns, self->goal - self->takedowns};
    Observable_notifyObservers(Score_asObservable(self), "sync_score", &args);
}