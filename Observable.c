#include "Observable.h"
#define MAX_OBSERVERS 10

typedef struct observable
{
    Observer *observers[MAX_OBSERVERS];
    int observersCount;
    void *implementationObj;
} Observable;

void Observable_notifyObservers(Observable *self, void *signalArgs)
{
    for (int i = 0; i < self->observersCount; i++)
    {
        Observer_notify(self->observers[i], self, signalArgs);
    }
}

void Observable_addObserver(Observable *self, Observer *o)
{
    if (!o)
        return;

    if (self->observersCount >= MAX_OBSERVERS)
    {
        perror("Maximum number of observers, cannot add another one. Aborting...\n");
        return;
    }

    self->observers[self->observersCount] = o;
    self->observersCount++;
}

void Observable_removeObserver(Observable *self, Observer *o)
{
    if (!o)
        return;

    if (self->observersCount <= 0)
    {
        perror("Specified observer not found, thus cannot be removed. Aborting...\n");
        return;
    }

    for (int i = 0; i < self->observersCount; i++)
    {
        if (self->observers[i] == o)
        {
            self->observers[i] = self->observers[self->observersCount - 1];
            self->observersCount--;
            return;
        }
    }

    perror("Specified observer not found, thus cannot be removed. Aborting...\n");
}

Observable *Observable_new(void *implementationObj)
{

    Observable *created = (Observable *)malloc(sizeof(Observable));
    created->observersCount = 0;
    created->implementationObj = implementationObj;


    return created;
}

void Observable_dispose(Observable *self)
{
    free(self);
}
