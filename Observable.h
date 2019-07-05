#ifndef OBSERVABLE_H
#define OBSERVABLE_H
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "Observer.h"

typedef struct observable Observable;

/** \memberof observable */
Observable* Observable_new(void *implementationObj);

/** \memberof observable */
void Observable_destroy(Observable* self);

/**
 *  \brief Wysyła sygnał do wszystkich obiektów obserwujących.
 * 
 *  \param signalID łańcuch znaków stanowiący krótki, często jednowyrazowy opis sygnału, np. "clicked".
 *  \param signalArgs dane, których mogą oczekiwać obiekty obserwujące. 
*/
void Observable_notifyObservers(Observable *self, const char* signalID, void *signalArgs);

/**
 * \memberof observable
 * \brief Dodaje obiekt do listy obserwujących.
*/
void Observable_addObserver(Observable *self, Observer* o);

/**
 * \memberof observable
 * \brief Usuwa obiekt z listy obserwujących.
*/
void Observable_removeObserver(Observable *self, Observer* o);




#endif