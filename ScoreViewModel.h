#pragma once
#include "Observable.h"

/**
 * \brief Model widoku wyniku.
 * 
 * Jest to prosta struktura zawierająca zewnętrzne informacje potrzebne do poprawnego wyświetlenia widoku.
*/
typedef struct score_view_model
{
    int points;                     //!< liczba zbitych pionków.
    int goal;                       //!< liczba pionków, które wyjściowo należało zbić, aby zwyciężyć.
    Observable* scoreObservable;    //!< obserwator modelu.
} ScoreViewModel;