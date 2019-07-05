#pragma once
#include "MatchView.h"
#include "MainMenuView.h"
#include "MatchViewModel.h"
#include "MainMenuViewModel.h"
#include "GameOverView.h"
#include "GameOverViewModel.h"
#include "IViewFactory.h"
typedef struct gtk_view_factory GtkViewFactory;

/** \memberof gtk_view_factory */
IViewFactory* GtkViewFactory_asIViewFactory(GtkViewFactory* self);

/** \memberof gtk_view_factory */
GtkViewFactory *GtkViewFactory_new();

/** \memberof gtk_view_factory */
void GtkViewFactory_destroy(GtkViewFactory *self);

/** \memberof gtk_view_factory */
MatchView *GtkViewFactory_createMatchView(GtkViewFactory *self, IGameController *controllerAPI, MatchViewModel vm);

/** \memberof gtk_view_factory */
MainMenuView *GtkViewFactory_createMainMenuView(GtkViewFactory *self, IGameController *controllerAPI, MainMenuViewModel vm);

/** \memberof gtk_view_factory */
GameOverView* GtkViewFactory_createGameOverView(GtkViewFactory * self, IGameController * controllerAPI, GameOverViewModel vm);