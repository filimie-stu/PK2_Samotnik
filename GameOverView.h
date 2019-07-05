#pragma once
#include <gtk/gtk.h>
#include "GameOverViewModel.h"
#include "IGameController.h"
#include "IView.h"

typedef struct game_over_view GameOverView;

/** \memberof game_over_view */
IView* GameOverView_asIView(GameOverView* self);

/** \memberof game_over_view */
GameOverView* GameOverView_new(IGameController* controllerAPI, GameOverViewModel viewModel);

/** \memberof game_over_view */
void GameOverView_destroy(GameOverView* self);

/** \memberof game_over_view */
void GameOverView_display(GameOverView* self);

/** \memberof game_over_view */
void GameOverView_hide(GameOverView* self);
