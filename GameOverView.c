    #include "GameOverView.h"
#include <stdlib.h>

typedef struct game_over_view 
{
    GtkWidget* dialog;
    GameController* controllerAPI;

} GameOverView;

void GameController_restartGame(GameController* controllerAPI);
void GameController_endMatch(GameController* controllerAPI);
void GameController_mainMenu(GameController* controllerAPI);

GameOverView* GameOverView_new(GameController* controllerAPI, GtkWindow* parent)
{
    GameOverView* created = (GameOverView*)malloc(sizeof(GameOverView)) ;
    GtkBuilder *builder = gtk_builder_new_from_file("view/game_over_view.glade");
    created->dialog = GTK_WIDGET(gtk_builder_get_object(builder, "gameOverDialog"));
    created->controllerAPI = controllerAPI;
    gtk_window_set_transient_for(GTK_WINDOW(created->dialog), parent);
    return created;
}
void GameOverView_destroy(GameOverView* self)
{
    free(self);
}

void GameOverView_display(GameOverView* self)
{
    gint dialogResponse = gtk_dialog_run(GTK_DIALOG(self->dialog));
    switch (dialogResponse)
    {
    case 0:
        GameController_restartGame(self->controllerAPI);
        break;
    case 1:
        GameController_endMatch(self->controllerAPI);
        GameController_mainMenu(self->controllerAPI);
        break;
    default:
        break;
    }
}
void GameOverView_hide(GameOverView* self)
{
    gtk_widget_destroy(self->dialog);
}