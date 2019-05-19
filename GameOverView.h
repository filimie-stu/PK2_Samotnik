#pragma once
#include <gtk/gtk.h>
#include "GameOverViewModel.h"
#include "IGameController.h"

typedef struct game_over_view GameOverView;


GameOverView* GameOverView_new(IGameController* controllerAPI, GtkWindow* parent, GameOverViewModel viewModel);
void GameOverView_destroy(GameOverView* self);
void GameOverView_display(GameOverView* self);
void GameOverView_hide(GameOverView* self);
