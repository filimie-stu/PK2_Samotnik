#pragma once
#include "BoardViewModel.h"
#include <gtk/gtk.h>
#include "IGameController.h"
typedef struct board_view BoardView;

/** \memberof board_view */
BoardView* BoardView_new(IGameController* controllerAPI, BoardViewModel board, GtkContainer* parent);
/** \memberof board_view */
void BoardView_destroy(BoardView* self);
/** \memberof board_view */
void BoardView_display(BoardView* self);
/** \memberof board_view */
void BoardView_hide(BoardView* self);