#pragma once
#include <gtk/gtk.h>
#include "IGameController.h"
#include "IView.h"
#include "MainMenuViewModel.h"
typedef struct main_menu_view MainMenuView;

MainMenuView* MainMenuView_new(IGameController* controllerAPI, MainMenuViewModel viewModel);
void MainMenuView_destroy(MainMenuView* self);
void MainMenuView_display(MainMenuView* self);
void MainMenuView_displayContinueButton(MainMenuView* self);
void MainMenuView_hideContinueButton(MainMenuView* self);
IView* MainMenuView_asIView(MainMenuView* self);
void MainMenuView_hide(MainMenuView* self);