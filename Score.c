#include "Score.h"
#include "SyncScoreArgs.h"
#include <stdlib.h>

Score *Score_new()
{
    Score *created = (Score *)malloc(sizeof(Score));
    created->goal = 10;
    created->takedowns = 0;
    created->observable = Observable_new(created);
    return created;
}
void Score_destroy(Score *self)
{
    free(self);
}

int private_goalAchieved(Score *self)
{
    return self->takedowns >= self->goal;
}
void Score_increment(Score *self)
{
    self->takedowns++;
    SyncScoreArgs args = {self->takedowns, self->goal - self->takedowns};
    Observable_notifyObservers(self->observable, "sync_score", &args);

    if (private_goalAchieved(self))
    {
        Observable_notifyObservers(self->observable, "goal_achieved", &args);
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
    Observable_notifyObservers(self->observable, "sync_score", &args);
}