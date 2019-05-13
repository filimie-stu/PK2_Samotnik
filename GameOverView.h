#pragma once
#include <gtk/gtk.h>

typedef struct game_controller GameController;
typedef struct game_over_view GameOverView;


GameOverView* GameOverView_new(GameController* controllerAPI, GtkWindow* parent);
void GameOverView_destroy(GameOverView* self);
void GameOverView_display(GameOverView* self);
void GameOverView_hide(GameOverView* self);