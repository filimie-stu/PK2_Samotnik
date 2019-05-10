#include "MatchView.h"
#include <stdlib.h>


void GameController_mainMenu(GameController* self);
void GameController_restartGame(GameController* self);

typedef struct match_view 
{
    GtkWidget* window;
    GtkWidget* mainMenuButton;
    GtkWidget* resetButton;
} MatchView;
static void private_mainMenu(GtkButton* button, gpointer data);
static void private_restartGame(GtkButton* button, gpointer data);

MatchView* MatchView_new(GameController* controllerAPI)
{
    MatchView* created = (MatchView*)malloc(sizeof(MatchView));
    
    GtkBuilder* builder = gtk_builder_new();
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "view/match_view.xml", &error) == 0)
    {
        g_printerr("Error during loading file: %s\n", error->message);
        g_clear_error(&error);
    }
    created->window = GTK_WIDGET(gtk_builder_get_object(builder, "matchWindow"));
    
    created->mainMenuButton = GTK_WIDGET(gtk_builder_get_object(builder, "mainMenuButton"));
    g_signal_connect(created->mainMenuButton, "clicked", G_CALLBACK(private_mainMenu), controllerAPI);

    created->resetButton = GTK_WIDGET(gtk_builder_get_object(builder, "resetButton"));
    g_signal_connect(created->mainMenuButton, "clicked", G_CALLBACK(private_restartGame), controllerAPI);

    return created;
}
void MatchView_destroy(MatchView* self)
{
    free(self);
}
void MatchView_display(MatchView* self)
{
    gtk_widget_show_all(self->window);
}
void MatchView_hide(MatchView* self)
{
    gtk_widget_hide(self->window);
}

void private_mainMenu(GtkButton* button, gpointer data)
{
    GameController_mainMenu((GameController*)data);
}
void private_restartGame(GtkButton* button, gpointer data)
{
    GameController_restartGame((GameController*)data);
}