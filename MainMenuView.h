#pragma once
#include <gtk/gtk.h>

typedef struct game_controller GameController;
typedef struct main_menu_view MainMenuView;


MainMenuView* MainMenuView_new(GameController* controllerAPI);
void MainMenuView_destroy(MainMenuView* self);
void MainMenuView_display(MainMenuView* self);
void MainMenuView_hide(MainMenuView* self);