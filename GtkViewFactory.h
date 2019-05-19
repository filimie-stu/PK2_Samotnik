#pragma once
#include "MatchView.h"
#include "MainMenuView.h"
#include "MatchViewModel.h"
#include "MainMenuViewModel.h"
#include "IViewFactory.h"
typedef struct gtk_view_factory GtkViewFactory;

IViewFactory* GtkViewFactory_asIViewFactory(GtkViewFactory* self);
GtkViewFactory *GtkViewFactory_new();
void GtkViewFactory_destroy(GtkViewFactory *self);
MatchView *GtkViewFactory_createMatchView(GtkViewFactory *self, IGameController *controllerAPI, MatchViewModel vm);
MainMenuView *GtkViewFactory_createMainMenuView(GtkViewFactory *self, IGameController *controllerAPI, MainMenuViewModel vm);