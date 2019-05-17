#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include "GameController.h"
#include "ModelFactory.h"

void openDialog(GtkButton* b, gpointer data)
{
    gtk_dialog_run((GtkDialog*)data);
}
int main (int argc, char* argv[])
{
    gtk_init(&argc, &argv);
    GameController* controller = GameController_new(
        ModelFactory_asIModelFactory(ModelFactory_new("data/board.txt"))
    );
    GameController_mainMenu(controller);



    // GtkBuilder* b = gtk_builder_new_from_file("view/game_over_view.glade");
    
    // GtkWidget* w = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    // GtkWidget* box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);


    // GtkWidget* btn = gtk_button_new_with_label("get dialog");

    // gtk_container_add(GTK_CONTAINER(w), box);
    // gtk_container_add(GTK_CONTAINER(box), btn);

    // GtkWidget *dialog = GTK_WIDGET(gtk_builder_get_object(b, "gameOverDialog"));
    // g_signal_connect(btn, "clicked", G_CALLBACK(openDialog), dialog);

    // g_signal_connect(dialog, "destroy", G_CALLBACK(gtk_widget_destroy), NULL);



    // gtk_widget_show_all(w);
    gtk_main();

    return 0;
}