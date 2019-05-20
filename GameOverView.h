#pragma once
#include <gtk/gtk.h>
#include "GameOverViewModel.h"
#include "IGameController.h"
#include "IView.h"

typedef struct game_over_view GameOverView;

IView* GameOverView_asIView(GameOverView* self);
GameOverView* GameOverView_new(IGameController* controllerAPI, GameOverViewModel viewModel);
void GameOverView_destroy(GameOverView* self);
void GameOverView_display(GameOverView* self);
void GameOverView_hide(GameOverView* self);
