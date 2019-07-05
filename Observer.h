#ifndef OBSERVER_H
#define OBSERVER_H
#include <stdlib.h>

typedef struct observable Observable;
typedef struct observer Observer;

/** \memberof observer */
Observer* Observer_new(void* implementationObj, void(*notifyOverride)(void* implObject, const char* sigID, void* sigArgs), Observable* src);

/** \memberof observer */
void Observer_destroy(Observer* self);

/** \memberof observer */
void Observer_notify(Observer* self, const char* signalID, void* signalArgs);


#endif