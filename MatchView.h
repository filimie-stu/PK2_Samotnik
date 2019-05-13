#pragma once
#include <gtk/gtk.h>
#include "Board.h"
#include "Score.h"
typedef struct game_controller GameController;
typedef struct match_view MatchView;

MatchView* MatchView_new(GameController* controllerAPI, Board* board, Score* score);

void MatchView_destroy(MatchView* self);
void MatchView_display(MatchView* self);
void MatchView_hide(MatchView* self);
