#include "Score.h"
#include "IScore.h"
#include "SyncScoreArgs.h"
#include <stdlib.h>

static int private_wrapper_getPoints(void* vSelf);
static int private_wrapper_getGoal(void* vSelf);
static void private_wrapper_increment(void* vSelf);
static void private_wrapper_decrement(void* vSelf);
static void private_wrapper_destroy(void* vSelf);

void private_wrapper_destroy(void* vSelf)
{
    Score_destroy((Score*)vSelf);
}
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

int private_wrapper_hasWon(void* vSelf)
{
    return Score_hasWon((Score*)vSelf);
}

Score *Score_new(int goal, int handicap)

{
    Score *created = (Score *)malloc(sizeof(Score));
    created->iScore = IScore_new(
        created,
        private_wrapper_getPoints,
        private_wrapper_getGoal,
        private_wrapper_increment,
        private_wrapper_decrement,
        private_wrapper_hasWon,
        private_wrapper_destroy
    );
    created->goal = goal;
    created->takedowns = 0;
    created->handicap = handicap;
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
    return self->goal - self->handicap;
}

int Score_hasWon(Score* self)
{
    return self->takedowns + self->handicap >= self->goal;
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