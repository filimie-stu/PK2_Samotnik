#include "Observer.h"
#include "Observable.h"

typedef struct observer
{
    void *implementationObj;
    notifyFnOverride notifyOverride;

} Observer;


Observer* Observer_new(void* implementationObj, notifyFnOverride notifyOverride, Observable* src)
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
void Observer_notify(Observer* self, Observable* signalSrc, void* signalArgs)
{
    self->notifyOverride(self->implementationObj, signalSrc, signalArgs);
}
