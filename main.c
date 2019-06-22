#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include "GameController.h"
#include "ModelFactory.h"
#include "GtkViewFactory.h"

int main (int argc, char* argv[])
{
    gtk_init(&argc, &argv);
    
    ModelFactory* modelFactory = ModelFactory_new();
    GtkViewFactory* viewFactory= GtkViewFactory_new();
    GameController* controller = GameController_new( 
        ModelFactory_asIModelFactory(modelFactory),
        GtkViewFactory_asIViewFactory(viewFactory)
        );

    GameController_mainMenu(controller);
    gtk_main();


    GameController_destroy(controller);
    ModelFactory_destroy(modelFactory);
    GtkViewFactory_destroy(viewFactory);
    return 0;
}