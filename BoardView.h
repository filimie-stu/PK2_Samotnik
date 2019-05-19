#pragma once
#include "BoardViewModel.h"
#include <gtk/gtk.h>
#include "IGameController.h"
typedef struct board_view BoardView;

BoardView* BoardView_new(IGameController* controllerAPI, BoardViewModel board, GtkContainer* parent);
void BoardView_destroy(BoardView* self);
void BoardView_display(BoardView* self);
void BoardView_hide(BoardView* self);