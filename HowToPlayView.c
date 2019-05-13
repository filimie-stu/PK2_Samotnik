#include "HowToPlayView.h"
#include <stdlib.h>


typedef struct how_to_play_view
{
    GameController* controllerAPI;
    GtkWidget* dialog;
} HowToPlayView;

HowToPlayView* HowToPlayView_new(GameController* controllerAPI, GtkWindow* parentWindow)
{
    HowToPlayView* created = (HowToPlayView*)malloc(sizeof(HowToPlayView));
    created->controllerAPI = controllerAPI;
    GtkBuilder *builder = gtk_builder_new_from_file("view/how_to_play_view.xml");
    created->dialog = GTK_WIDGET(gtk_builder_get_object(builder, "howToPlayDialog"));
    gtk_window_set_transient_for(GTK_WINDOW(created->dialog), parentWindow);

    return created;
}
void HowToPlayView_destroy(HowToPlayView* self)
{
    free(self);
}
void HowToPlayView_display(HowToPlayView* self)
{
    gint dialogResponse = gtk_dialog_run(GTK_DIALOG(self->dialog)); 
    HowToPlayView_hide(self);
}
void HowToPlayView_hide(HowToPlayView* self)
{
    gtk_widget_hide(self->dialog);
}