#include "MainMenuView.h"
#include <stdlib.h>


void GameController_prepareForExit(GameController* self);
void GameController_mainMenu(GameController* self);
void GameController_beginMatch(GameController* self);

typedef struct main_menu_view 
{
    GtkWidget* window;
    GtkWidget* newGameBtn;
    GtkWidget* exitBtn;
    GameController* controllerAPI;

} MainMenuView;

static void private_beginMatch(GtkButton* buttonr, gpointer data);
static void private_exitProgram(GtkButton* button, gpointer data);



MainMenuView* MainMenuView_new(GameController* controllerAPI)
{
    MainMenuView* created = (MainMenuView*)malloc(sizeof(MainMenuView));
    created->controllerAPI = controllerAPI;

    GtkBuilder* builder = gtk_builder_new();
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "view/main_menu_view.xml", &error) == 0)
    {
        g_printerr("Error during loading file: %s\n", error->message);
        g_clear_error(&error);
    }
    created->window = GTK_WIDGET(gtk_builder_get_object(builder, "mainMenuWindow"));
    created->exitBtn = GTK_WIDGET(gtk_builder_get_object(builder, "exitButton"));
    created->newGameBtn = GTK_WIDGET(gtk_builder_get_object(builder, "newGameButton"));

    g_signal_connect(created->exitBtn, "clicked", G_CALLBACK(private_exitProgram), controllerAPI);
    g_signal_connect(created->newGameBtn, "clicked", G_CALLBACK(private_beginMatch), controllerAPI);
    
    return created;
}
void MainMenuView_destroy(MainMenuView* self)
{
    gtk_widget_destroy(self->window);
    free(self);
}
void MainMenuView_display(MainMenuView* self)
{
    gtk_widget_show_all(self->window);
}
void MainMenuView_hide(MainMenuView* self)
{
    gtk_widget_hide(self->window);
}

void private_beginMatch(GtkButton* button, gpointer data)
{
    GameController_beginMatch((GameController*)data);
}

void private_exitProgram(GtkButton* button, gpointer data)
{
    GameController_prepareForExit((GameController*)data);
    gtk_main_quit();
}