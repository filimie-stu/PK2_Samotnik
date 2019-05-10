#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include "GameController.h"


int main(int argc, char* argv[])
{
    gtk_init(&argc, &argv);
    GameController* controller = GameController_new();
    GameController_mainMenu(controller);


    gtk_main();
    return 0;
}