#include "HowToPlayDialog.h"
#include <stdlib.h>

/**
 * \brief Okno dialogowe zawierające instrukcję gry.
*/
typedef struct how_to_play_view
{
    GtkWidget* dialog;                  //!< Wskaźnik na obiekt biblioteki GTK+ reprezentujący okno dialogowe.
} HowToPlayDialog;

HowToPlayDialog* HowToPlayDialog_newFromFile(const char* relativeFilename, GtkWindow *parentWindow)
{
    HowToPlayDialog* created = (HowToPlayDialog*)malloc(sizeof(HowToPlayDialog));
    GtkBuilder *builder = gtk_builder_new_from_file("view/how_to_play_view.glade");
    created->dialog = GTK_WIDGET(gtk_builder_get_object(builder, "howToPlayDialog"));
    g_object_unref(builder);
    
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