#include "Observer.h"
#include "Observable.h"

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
void Observer_dispose(Observer* self)
{
    free(self);
}
void Observer_notify(Observer* self, const char* signalID, void* signalArgs)
{
    self->notifyOverride(self->implementationObj, signalID, signalArgs);
}
