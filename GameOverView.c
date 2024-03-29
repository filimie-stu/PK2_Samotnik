#include "Vector2D.h"
#include "GameOverView.h"
#include "GameOverViewModel.h"
#include <stdlib.h>
#include "IView.h"

/**
 * \brief Widok powiadomienia o zakończeniu gry. 
 * 
 * Odpowiada za wyświetlanie okna z wiadomością o zwycięstwie bądź porażce.
*/
typedef struct game_over_view
{
    IView *iView;                       //!< implementacja interfejsu IView
    GtkWidget *window;                  //!< wskaźnik na okno gry (nadrzędne względem okna powiadomienia)
    IGameController *controllerAPI;     //!< publiczny interfejs kontrolera
} GameOverView;

static void private_wrapper_destroy(void *vSelf);
static void private_wrapper_display(void *vSelf);
static void private_wrapper_hide(void *vSelf);

void GameOverView_destroy(GameOverView *self)
{
    gtk_widget_destroy(self->window);
    IView_destroy(self->iView, 0);
    free(self);
}

void private_wrapper_destroy(void *vSelf)
{
    GameOverView_destroy((GameOverView*)vSelf);
}
void private_wrapper_display(void *vSelf)
{
    GameOverView_display((GameOverView*)vSelf);
}

void private_wrapper_hide(void *vSelf)
{
    GameOverView_hide((GameOverView*)vSelf);
}

IView *GameOverView_asIView(GameOverView *self)
{
    return self->iView;
}
GameOverView *GameOverView_new(IGameController *controllerAPI, GameOverViewModel viewModel)
{
    GameOverView *created = (GameOverView *)malloc(sizeof(GameOverView));
    created->iView = IView_new(
        created,
        private_wrapper_destroy,
        private_wrapper_display,
        private_wrapper_hide);
    created->controllerAPI = controllerAPI;

    GtkBuilder *builder = gtk_builder_new_from_file("view/game_over_view.glade");

    created->window = GTK_WIDGET(gtk_builder_get_object(builder, "gameOverWindow"));

    GtkWidget *primaryLabel = GTK_WIDGET(gtk_builder_get_object(builder, "primaryText"));
    gtk_label_set_label(GTK_LABEL(primaryLabel), viewModel.primaryText);

    GtkWidget *detailedDescription = GTK_WIDGET(gtk_builder_get_object(builder, "detailedDescription"));
    gtk_label_set_label(GTK_LABEL(detailedDescription), viewModel.detailedDescription);

    GtkWidget* restartBtn = GTK_WIDGET(gtk_builder_get_object(builder,"restartBtn"));
    GtkWidget* mainMenuBtn = GTK_WIDGET(gtk_builder_get_object(builder,"mainMenuBtn"));

    g_object_unref(builder);
    g_signal_connect_swapped(restartBtn, "clicked", G_CALLBACK(IGameController_restartGame), created->controllerAPI);
    g_signal_connect_swapped(mainMenuBtn, "clicked", G_CALLBACK(IGameController_endMatch), created->controllerAPI);

    return created;
}
void GameOverView_display(GameOverView *self)
{
    gtk_widget_show_all(self->window);
}
void GameOverView_hide(GameOverView *self)
{
    gtk_widget_hide(self->window);
}