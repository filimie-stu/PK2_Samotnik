#pragma once
#include "IScore.h"

typedef struct score Score;

/** \memberof score */ 
Score* Score_new(int goal, int handicap);

/** \memberof score */ 
IScore* Score_asIScore(Score* self);

/** \memberof score */ 
Observable* Score_asObservable(Score* self);

/** \memberof score */ 
void Score_destroy(Score* self);

/** \memberof score */ 
void Score_increment(Score* self);

/** \memberof score */ 
void Score_decrement(Score *self);

/** 
 * \memberof score 
 * \brief Akcesor liczby zbitych pionków. 
 * */ 
int Score_getPoints(Score* self);

/** \memberof score 
 * \brief Akcesor liczby zbitych pionków gwarantującej zwycięstwo (z uwzględnieniem pionków pomocniczych).
 * */ 
int Score_getGoal(Score* self);

/** \memberof score 
 *  \brief Sprawdzenie czy gracz już zwyciężył.
 *  \return 1 jeżeli gracz zwyciężył, w przeciwnym razie - 0.
 * */ 
int Score_hasWon(Score* self);