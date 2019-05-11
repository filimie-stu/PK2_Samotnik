#ifndef OBSERVER_H
#define OBSERVER_H
#include <stdlib.h>

typedef struct observable Observable;
typedef void (*notifyFnOverride)(void* implementationObj, Observable * signalSrc, void * signalArgs); 
typedef struct observer Observer;

Observer* Observer_new(void* implementationObj, notifyFnOverride notifyOverride, Observable* src);
void Observer_dispose(Observer* self);
void Observer_notify(Observer* self, Observable* signalSrc, void* signalArgs);


#endif