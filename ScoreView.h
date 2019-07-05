#pragma once
#include <gtk/gtk.h>
#include "ScoreViewModel.h"
#include "SyncScoreArgs.h"
#include "IGameController.h"

typedef struct score_view ScoreView;

/** \memberof score_view */
ScoreView* ScoreView_new(IGameController* controllerAPI, ScoreViewModel score, GtkContainer* parent);

/** \memberof score_view */
void ScoreView_destroy(ScoreView* self);
