#pragma once
#include <gtk/gtk.h>
#include "ScoreViewModel.h"
#include "SyncScoreArgs.h"
typedef struct game_controller GameController;
typedef struct score_view ScoreView;


ScoreView* ScoreView_new(GameController* controllerAPI, ScoreViewModel score, GtkContainer* parent);
void ScoreView_destroy(ScoreView* self);
void ScoreView_display(ScoreView* self);
void ScoreView_hide(ScoreView* self);
// void ScoreView_syncScore(ScoreView* self, SyncScoreArgs args);