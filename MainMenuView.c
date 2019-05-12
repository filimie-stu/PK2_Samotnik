#include "MainMenuView.h"
#include <stdlib.h>


void GameController_prepareForExit(GameController* self);
void GameController_mainMenu(GameController* self);
void GameController_beginMatch(GameController* self);
void GameController_continueMatch(GameController* self);

typedef struct main_menu_view 
{
    GtkWidget* window;
    GtkWidget* continueBtn;
    GtkWidget* newGameBtn;
    GtkWidget* exitBtn;
    GameController* controllerAPI;

} MainMenuView;

static void private_beginMatch(GtkButton* button, gpointer data);
static void private_exitProgram(GtkButton* button, gpointer data);
static void private_continueMatch(GtkButton* button, gpointer data);

void private_continueMatch(GtkButton* button, gpointer data)
{
    GameController_continueMatch((GameController*)data);
}

MainMenuView* MainMenuView_new(GameController* controllerAPI)
{
    MainMenuView* created = (MainMenuView*)malloc(sizeof(MainMenuView));
    created->controllerAPI = controllerAPI;

    GtkBuilder* builder = gtk_builder_new();
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "view/main_menu_view.glade", &error) == 0)
    {
        g_printerr("Error during loading file: %s\n", error->message);
        g_clear_error(&error);
    }
    created->window = GTK_WIDGET(gtk_builder_get_object(builder, "mainMenuWindow"));
    created->exitBtn = GTK_WIDGET(gtk_builder_get_object(builder, "exitBtn"));
    created->newGameBtn = GTK_WIDGET(gtk_builder_get_object(builder, "newGameBtn"));
    created->continueBtn = GTK_WIDGET(gtk_builder_get_object(builder, "continueBtn"));
    g_signal_connect(created->exitBtn, "clicked", G_CALLBACK(private_exitProgram), controllerAPI);
    g_signal_connect(created->newGameBtn, "clicked", G_CALLBACK(private_beginMatch), controllerAPI);
    g_signal_connect(created->continueBtn, "clicked", G_CALLBACK(private_continueMatch), controllerAPI);
    
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
void MainMenuView_displayContinueButton(MainMenuView* self)
{
    gtk_widget_show(self->continueBtn);
}
void MainMenuView_hideContinueButton(MainMenuView* self)
{
    gtk_widget_hide(self->continueBtn);
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
