#pragma once
#include <gtk/gtk.h>

typedef struct game_controller GameController;
typedef struct match_view MatchView;

MatchView* MatchView_new(GameController* controllerAPI);
void MatchView_destroy(MatchView* self);
void MatchView_display(MatchView* self);
void MatchView_hide(MatchView* self);
