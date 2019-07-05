#pragma once
#include "Vector2D.h"

typedef struct board_view BoardView;


/**
 * \brief Struktura grupująca dane dotyczące kliknięcia w pole planszy.
 * 
 * Struktura ta jest wykorzystywana wyłącznie przez 'BoardView'.
 * \sa board_view
*/
typedef struct view_click_data 
{
    BoardView* view;    //!< wskaźnik na widok planszy
    Vector2D coords;    //!< współrzędne klikniętego pola
} ViewClickData;

/* \memberof view_click_data */
ViewClickData* ViewClickData_new(BoardView* view, Vector2D coords);

/* \memberof view_click_data */
void ViewClickData_destroy(ViewClickData* self);