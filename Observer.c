#include "Observer.h"
#include "Observable.h"


/**
 * \brief Klasa abstrakcyjna implementowana przez obiekty stanowiące obserwatorów, oczekujących na różnego typu sygnały.
 * 
 * Klasy, które mogą emitować sygnały winny dziedziczyć po komplementarnej klasie abstrakcyjnej 'Observable'.
 * 
 * \sa observable
*/
typedef struct observer
{
    void *implementationObj;
    void (*notifyOverride)(void* implementationObj, const char* signalID, void * signalArgs); 

} Observer;


Observer* Observer_new(void* implementationObj, void(*notifyOverride)(void* implObject, const char* sigID, void* sigArgs), Observable* src)
{
    Observer* created = (Observer*)malloc(sizeof(Observer));
    created->implementationObj = implementationObj;
    created->notifyOverride = notifyOverride;

    Observable_addObserver(src, created);
    
    return created;
}
void Observer_destroy(Observer* self)
{
    free(self);
}
void Observer_notify(Observer* self, const char* signalID, void* signalArgs)
{
    self->notifyOverride(self->implementationObj, signalID, signalArgs);
}
