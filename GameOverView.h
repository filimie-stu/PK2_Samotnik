#pragma once
#include <gtk/gtk.h>
#include "GameOverViewModel.h"
typedef struct game_controller GameController;
typedef struct game_over_view GameOverView;


GameOverView* GameOverView_new(GameController* controllerAPI, GtkWindow* parent, GameOverViewModel viewModel);
void GameOverView_destroy(GameOverView* self);
void GameOverView_display(GameOverView* self);
void GameOverView_hide(GameOverView* self);
void GameController_activate(GameController* self, Vector2D at);
void GameController_jump(GameController* self, Vector2D from, Vector2D to);
