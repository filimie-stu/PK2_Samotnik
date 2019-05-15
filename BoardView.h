#pragma once
#include "Board.h"
#include <gtk/gtk.h>
typedef struct game_controller GameController;
typedef struct board_view BoardView;

BoardView* BoardView_new(GameController* controllerAPI, Board* board, GtkContainer* parent);
void BoardView_destroy(BoardView* self);
void BoardView_display(BoardView* self);
void BoardView_hide(BoardView* self);