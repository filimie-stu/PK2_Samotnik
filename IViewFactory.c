#include "IViewFactory.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct i_view_factory
{
    void *implementationObject;
    void (*destroyOverride)(void *implSelf);
    IView *(*createMatchViewOverride)(void *implSelf, IGameController *controllerAPI, MatchViewModel vm);
    IView *(*createMainMenuViewOverride)(void *implSelf, IGameController *controllerAPI, MainMenuViewModel vm);
} IViewFactory;

IViewFactory *IViewFactory_new(
    void *implementationObject,
    void (*destroyOverride)(void *implSelf),
    IView *(*createMatchViewOverride)(void *implSelf, IGameController *controllerAPI, MatchViewModel vm),
    IView *(*createMainMenuViewOverride)(void *implSelf, IGameController *controllerAPI, MainMenuViewModel vm))
{
    if (!implementationObject || !destroyOverride || !createMatchViewOverride || !createMainMenuViewOverride)
    {
        printf("Error: NULL passed as interface override.\n");
    }
    IViewFactory *created = (IViewFactory *)malloc(sizeof(IViewFactory));
    created->implementationObject = implementationObject;
    created->destroyOverride = destroyOverride;
    created->createMatchViewOverride = createMatchViewOverride;
    created->createMainMenuViewOverride = createMainMenuViewOverride;

    return created;
}

IView *IViewFactory_createMatchView(IViewFactory *self, IGameController *controllerAPI, MatchViewModel vm)
{
    return self->createMatchViewOverride(self->implementationObject, controllerAPI, vm);
}

IView *IViewFactory_createMainMenuView(IViewFactory *self, IGameController *controllerAPI, MainMenuViewModel vm)
{
    return self->createMainMenuViewOverride(self->implementationObject, controllerAPI, vm);
}

void IViewFactory_destroy(IViewFactory *self, int destroyDerivedTypes)
{
    if (destroyDerivedTypes)
    {
        self->destroyOverride(self->implementationObject);
    }
    else
    {
        free(self);
    }
}