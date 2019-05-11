#include "Score.h"
#include "SyncScoreArgs.h"
#include <stdlib.h>

Score* Score_new()
{
    Score* created = (Score*)malloc(sizeof(Score));
    created->goal = 31;
    created->takedowns = 0;
    created->observable = Observable_new(created); 
    return created;
}
void Score_destroy(Score* self)
{
    free(self);
}
void Score_increment(Score* self)
{
    self->takedowns++;
    SyncScoreArgs args = { self->takedowns, self->goal - self->takedowns };
    Observable_notifyObservers(self->observable, "sync_score", &args);
}