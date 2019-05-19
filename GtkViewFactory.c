#include "GtkViewFactory.h"
#include "IViewFactory.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct gtk_view_factory
{
    IViewFactory *iViewFactory;

} GtkViewFactory;

static void private_wrapper_destroy(void *vSelf);
static IView *private_wrapper_createMatchView(void *vSelf, IGameController *controllerAPI, MatchViewModel vm);
static IView *private_wrapper_createMainMenuView(void *vSelf, IGameController *controllerAPI, MainMenuViewModel vm);
IViewFactory* GtkViewFactory_asIViewFactory(GtkViewFactory* self)
{
    return self->iViewFactory;  
}
static void private_wrapper_destroy(void *vSelf)
{
    GtkViewFactory_destroy((GtkViewFactory *)vSelf);
}
static IView *private_wrapper_createMatchView(void *vSelf, IGameController *controllerAPI, MatchViewModel vm)
{
    return MatchView_asIView(
        GtkViewFactory_createMatchView((GtkViewFactory *)vSelf, controllerAPI, vm));
}
static IView *private_wrapper_createMainMenuView(void *vSelf, IGameController *controllerAPI, MainMenuViewModel vm)
{
    return MainMenuView_asIView(
        GtkViewFactory_createMainMenuView((GtkViewFactory *)vSelf, controllerAPI, vm));
}

GtkViewFactory *GtkViewFactory_new()
{
    GtkViewFactory *created = (GtkViewFactory *)malloc(sizeof(GtkViewFactory));
    created->iViewFactory = IViewFactory_new(
        created,
        private_wrapper_destroy,
        private_wrapper_createMatchView,
        private_wrapper_createMainMenuView);

    return created;
}
void GtkViewFactory_destroy(GtkViewFactory *self)
{
    //todo
}
MatchView *GtkViewFactory_createMatchView(GtkViewFactory *self, IGameController *controllerAPI, MatchViewModel vm)
{
    return MatchView_new(controllerAPI, vm);
}
MainMenuView *GtkViewFactory_createMainMenuView(GtkViewFactory *self, IGameController *controllerAPI, MainMenuViewModel vm)
{
    return MainMenuView_new(controllerAPI, vm);
}