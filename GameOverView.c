#include "Vector2D.h"
#include "GameOverView.h"
#include "GameOverViewModel.h"
#include <stdlib.h>

typedef struct game_over_view 
{
    GtkWidget* dialog;
    IGameController* controllerAPI;

} GameOverView;

GameOverView* GameOverView_new(IGameController* controllerAPI, GtkWindow* parent, GameOverViewModel viewModel)
{
    GameOverView* created = (GameOverView*)malloc(sizeof(GameOverView)) ;
    GtkBuilder *builder = gtk_builder_new_from_file("view/game_over_view.glade");
    created->dialog = GTK_WIDGET(gtk_builder_get_object(builder, "gameOverDialog"));

    GtkWidget* primaryLabel = GTK_WIDGET(gtk_builder_get_object(builder, "primaryText"));
    gtk_label_set_label(GTK_LABEL(primaryLabel), viewModel.primaryText);
    
    GtkWidget* detailedDescription = GTK_WIDGET(gtk_builder_get_object(builder, "detailedDescription"));
    gtk_label_set_label(GTK_LABEL(detailedDescription), viewModel.detailedDescription);

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
        IGameController_restartGame(self->controllerAPI);
        break;
    case 1:
        IGameController_endMatch(self->controllerAPI);
        IGameController_mainMenu(self->controllerAPI);
        break;
    default:
        break;
    }
}
void GameOverView_hide(GameOverView* self)
{
    gtk_widget_destroy(self->dialog);
}