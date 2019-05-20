#ifndef OBSERVABLE_H
#define OBSERVABLE_H
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "Observer.h"

typedef struct observable Observable;


Observable* Observable_new(void *implementationObj);

void Observable_destroy(Observable* self);
void Observable_notifyObservers(Observable *self, const char* signalID, void *signalArgs);
void Observable_addObserver(Observable *self, Observer* o);
void Observable_removeObserver(Observable *self, Observer* o);




#endif