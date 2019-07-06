#include "MatchView.h"
#include "GameOverView.h"
#include "BoardView.h"
#include "ScoreView.h"
#include "BoardViewModel.h"
#include "Observer.h"
#include "SyncScoreArgs.h"
#include <stdlib.h>
#include <string.h>

/**
 * \brief Główny widok gry.
 * 
 * Klasa ta odpowiada za wizualną prezentację rozgrywki. Składa sie z dwóch subwidoków: BoardView oraz ScoreView,
 * których zakres odpowiedzialności skupia się wokół prezentacji odpowiednio: planszy oraz aktualnego wyniku.
*/
typedef struct match_view
{
    IView *iView;                   //!< implementacja interfejsu IView.
    IGameController *controllerAPI; //!< publiczny interfejs kontrolera gry

    GtkWidget *window;         //!< wskaźnik na obiekt biblioteki GTK+ reprezentujący główne okno progamu.
    GtkWidget *mainMenuButton; //!< wskaźnik na obiekt biblioteki GTK+ reprezentujący przycisk przejścia do głównego menu.
    GtkWidget *resetButton;    //!< wskaźnik na obiekt biblioteki GTK+ reprezentujący przycisk ponownego rozpoczęcia gry.
    GtkWidget *rollbackButton; //!< wskaźnik na obiekt biblioteki GTK+ reprezentujący przycisk cofnięcia ruchu.

    GtkContainer *boardAnchorPoint; //!< wskaźnik na obiekt biblioteki GTK+ reprezentujący miejsce "zaczepienia" subwidoku planszy.
    BoardView *boardView;           //!< widok planszy.

    GtkContainer *scoreAnchorPoint; //!< wskaźnik na obiekt biblioteki GTK+ reprezentujący miejsce "zaczepienia" subwidoku wyniku.
    ScoreView *scoreView;           //!< widok wyniku.

} MatchView;

static void private_mainMenu(GtkButton *button, gpointer data);
static void private_restartGame(GtkButton *button, gpointer data);
static void private_rollbackJump(GtkButton *button, gpointer data);
static void private_wrapper_destroy(void *vSelf);
static void private_wrapper_display(void *vSelf);
static void private_wrapper_hide(void *vSelf);
static void private_forcedExit(MatchView *self);
static void private_loadMembersFromXML(MatchView *self);
static void private_configureCallbacks(MatchView *self);

IView *MatchView_asIView(MatchView *self)
{
    return self->iView;
}

MatchView *MatchView_new(IGameController *controllerAPI, MatchViewModel viewModel)
{
    MatchView *created = (MatchView *)malloc(sizeof(MatchView));
    created->iView = IView_new(
        created,
        private_wrapper_destroy,
        private_wrapper_display,
        private_wrapper_hide);

    created->controllerAPI = controllerAPI;

    private_loadMembersFromXML(created);

    created->boardView = BoardView_new(created->controllerAPI, viewModel.boardVM, created->boardAnchorPoint);
    created->scoreView = ScoreView_new(created->controllerAPI, viewModel.scoreVM, created->scoreAnchorPoint);

    private_configureCallbacks(created);

    return created;
}
void MatchView_destroy(MatchView *self)
{
    BoardView_destroy(self->boardView);
    ScoreView_destroy(self->scoreView);
    gtk_widget_destroy(self->window);
    IView_destroy(self->iView, 0);
    free(self);
}
void MatchView_display(MatchView *self)
{
    gtk_widget_show_all(self->window);
}
void MatchView_hide(MatchView *self)
{
    gtk_widget_hide(self->window);
}


void private_loadMembersFromXML(MatchView *self)
{
    GtkBuilder *builder = gtk_builder_new_from_file("view/match_view.glade");
    self->window = GTK_WIDGET(gtk_builder_get_object(builder, "matchWindow"));
    self->mainMenuButton = GTK_WIDGET(gtk_builder_get_object(builder, "mainMenuBtn"));
    self->resetButton = GTK_WIDGET(gtk_builder_get_object(builder, "restartBtn"));
    self->rollbackButton = GTK_WIDGET(gtk_builder_get_object(builder, "rollbackBtn"));
    self->boardAnchorPoint = GTK_CONTAINER(gtk_builder_get_object(builder, "boardAnchorPoint"));
    self->scoreAnchorPoint = GTK_CONTAINER(gtk_builder_get_object(builder, "scoreAnchorPoint"));
    g_object_unref(builder);
}
void private_forcedExit(MatchView *self)
{
    IGameController_prepareForExit(self->controllerAPI);
    gtk_main_quit();
}

void private_configureCallbacks(MatchView *self)
{
    g_signal_connect(self->mainMenuButton, "clicked", G_CALLBACK(private_mainMenu), self->controllerAPI);
    g_signal_connect(self->resetButton, "clicked", G_CALLBACK(private_restartGame), self->controllerAPI);
    g_signal_connect(self->rollbackButton, "clicked", G_CALLBACK(private_rollbackJump), self->controllerAPI);
    g_signal_connect_swapped(self->window, "delete-event", G_CALLBACK(private_forcedExit), self);
}
void private_mainMenu(GtkButton *button, gpointer data)
{
    IGameController_mainMenu((IGameController *)data);
}
void private_restartGame(GtkButton *button, gpointer data)
{
    IGameController_restartGame((IGameController *)data);
}
void private_rollbackJump(GtkButton *button, gpointer data)
{
    IGameController_rollback((IGameController *)data);
}
void private_wrapper_destroy(void *vSelf)
{
    MatchView_destroy((MatchView *)vSelf);
}
void private_wrapper_display(void *vSelf)
{
    MatchView_display((MatchView *)vSelf);
}
void private_wrapper_hide(void *vSelf)
{
    MatchView_hide((MatchView *)vSelf);
}
