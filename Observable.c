#include "Observable.h"
#define MAX_OBSERVERS 10

/**
 * \brief Klasa abstrakcyjna implementowana przez obiekty będące źródłami sygnałów.
 * 
 * Każdy obiekt, który z jakiegoś powodu oczekuje na sygnały wysyłane przez implementację 'Observable'
 * powinien dziedziczyć po komplementarnej klasie 'Observer', której instancja może zostać dodana 
 * do listy subskrybentów obiektu 'Observable'.  
 * 
 * \sa observer
*/
typedef struct observable
{
    Observer *observers[MAX_OBSERVERS];     //!< tablica obiektów obserwujących.
    int observersCount;                     //!< liczba obiektów obserwujących
    void *implementationObj;            
} Observable;

void Observable_notifyObservers(Observable *self, const char* signalID, void *signalArgs)
{
    for (int i = 0; i < self->observersCount; i++)
    {
        Observer_notify(self->observers[i], signalID, signalArgs);
    }
}

void Observable_addObserver(Observable *self, Observer *o)
{
    if (!o)
        return;

    if (self->observersCount >= MAX_OBSERVERS)
    {
        perror("Warning: Maximum number of observers, cannot add another one. Aborting...\n");
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
        perror("Warning: Specified observer not found, thus cannot be removed. Aborting...\n");
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

void Observable_destroy(Observable *self)
{
    free(self);
}
