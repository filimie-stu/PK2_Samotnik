#pragma once
#include <gtk/gtk.h>
#include "ScoreViewModel.h"
#include "SyncScoreArgs.h"
#include "IGameController.h"

typedef struct score_view ScoreView;

ScoreView* ScoreView_new(IGameController* controllerAPI, ScoreViewModel score, GtkContainer* parent);
void ScoreView_destroy(ScoreView* self);
