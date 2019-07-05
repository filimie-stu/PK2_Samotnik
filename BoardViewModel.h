#pragma once
#include "Vector2D.h"
#include "Observable.h"
#include "Common.h"

#define MAX_DIM_X 100
#define MAX_DIM_Y 100

/**
 * \brief Zbiór danych potrzebnych do utworzenia instacji BoardView na podstawie instacji Board.
*/
typedef struct board_view_model
{
    Observable* boardObservable;                    //< implementacja interfejsu obiektu obserwowanego instacji Board
    Vector2D dimensions;                            //< wymiary planszy 
    FieldType fields[MAX_DIM_X][MAX_DIM_Y];         //< dwuwymiarowa tablica typów pól planszy  

} BoardViewModel;

