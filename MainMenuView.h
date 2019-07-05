#pragma once
#include <gtk/gtk.h>
#include "IGameController.h"
#include "IView.h"
#include "MainMenuViewModel.h"
typedef struct main_menu_view MainMenuView;

/** \memberof main_menu_view */
MainMenuView* MainMenuView_new(IGameController* controllerAPI, MainMenuViewModel viewModel);

/** \memberof main_menu_view */
void MainMenuView_destroy(MainMenuView* self);

/** \memberof main_menu_view */
void MainMenuView_display(MainMenuView* self);

/** \memberof main_menu_view */
void MainMenuView_displayContinueButton(MainMenuView* self);

/** \memberof main_menu_view */
void MainMenuView_hideContinueButton(MainMenuView* self);

/** \memberof main_menu_view */
IView* MainMenuView_asIView(MainMenuView* self);

/** \memberof main_menu_view */
void MainMenuView_hide(MainMenuView* self);