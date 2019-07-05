#pragma once
#include <gtk/gtk.h>
#include "IView.h"
#include "BoardViewModel.h"
#include "ScoreViewModel.h"
#include "MatchViewModel.h"
#include "IGameController.h"
typedef struct match_view MatchView;

/** \memberof match_view */
MatchView* MatchView_new(IGameController* controlleAPI, MatchViewModel viewModel);

/** \memberof match_view */
IView* MatchView_asIView(MatchView* self);

/** \memberof match_view */
void MatchView_destroy(MatchView* self);

/** \memberof match_view */
void MatchView_display(MatchView* self);

/** \memberof match_view */
void MatchView_hide(MatchView* self);
