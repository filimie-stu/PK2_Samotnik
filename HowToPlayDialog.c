#include "HowToPlayDialog.h"
#include <stdlib.h>


typedef struct how_to_play_view
{
    GameController* controllerAPI;
    GtkWidget* dialog;
} HowToPlayDialog;

HowToPlayDialog* HowToPlayDialog_newFromFile(const char* relativeFilename, GtkWindow *parentWindow)
{
    HowToPlayDialog* created = (HowToPlayDialog*)malloc(sizeof(HowToPlayDialog));
    
    GtkBuilder *builder = gtk_builder_new_from_file("view/how_to_play_view.glade");
    created->dialog = GTK_WIDGET(gtk_builder_get_object(builder, "howToPlayDialog"));
    gtk_window_set_transient_for(GTK_WINDOW(created->dialog), parentWindow);
    

    return created;
}

void HowToPlayDialog_destroy(HowToPlayDialog* self)
{
    free(self);
}

GtkWidget* HowToPlayDialog_getDialog(HowToPlayDialog* self)
{
    return self->dialog;
}