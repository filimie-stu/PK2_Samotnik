#pragma once
#include <gtk/gtk.h>
#include "IView.h"
#include "BoardViewModel.h"
#include "ScoreViewModel.h"
#include "MatchViewModel.h"
#include "IGameController.h"
typedef struct match_view MatchView;

MatchView* MatchView_new(IGameController* controlleAPI, MatchViewModel viewModel);
IView* MatchView_asIView(MatchView* self);
void MatchView_destroy(MatchView* self);
void MatchView_display(MatchView* self);
void MatchView_hide(MatchView* self);
