#pragma once
#include "IView.h"
#include "MatchViewModel.h"
#include "MainMenuViewModel.h"

typedef struct i_view_factory IViewFactory;

IViewFactory *IViewFactory_new(
    void *implementationObject,
    void (*destroyOverride)(void *implSelf),
    IView *(*createMatchViewOverride)(void *implSelf, IGameController *controllerAPI, MatchViewModel vm),
    IView *(*createMainMenuViewOverride)(void *implSelf, IGameController *controllerAPI, MainMenuViewModel vm));
IView *IViewFactory_createMatchView(IViewFactory *self, IGameController *controllerAPI, MatchViewModel vm);
IView *IViewFactory_createMainMenuView(IViewFactory *self, IGameController *controllerAPI, MainMenuViewModel vm);
void IViewFactory_destroy(IViewFactory *self, int destroyDerivedTypes);