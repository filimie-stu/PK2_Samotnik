#include "MatchView.h"
#include "GameOverView.h"
#include "BoardView.h"
#include "ScoreView.h"
#include "BoardViewModel.h"
#include "Observer.h"
#include "SyncScoreArgs.h"
#include <stdlib.h>
#include <string.h>


typedef struct match_view
{
    IView *iView;
    IGameController *controllerAPI;

    GtkWidget *window;
    GtkWidget *mainMenuButton;
    GtkWidget *resetButton;
    GtkWidget *rollbackButton;

    GtkContainer *boardAnchorPoint;
    BoardView *boardView;

    GtkContainer *scoreAnchorPoint;
    ScoreView *scoreView;


} MatchView;

static void private_mainMenu(GtkButton *button, gpointer data);
static void private_restartGame(GtkButton *button, gpointer data);
static void private_rollbackJump(GtkButton *button, gpointer data);
static void private_wrapper_destroy(void *vSelf);
static void private_wrapper_display(void *vSelf);
static void private_wrapper_hide(void *vSelf);

IView *MatchView_asIView(MatchView *self)
{
    return self->iView;
}
void private_loadMembersFromXML(MatchView* self)
{
    GtkBuilder *builder = gtk_builder_new_from_file("view/in_game_view.glade");
    self->window = GTK_WIDGET(gtk_builder_get_object(builder, "matchWindow"));
    self->mainMenuButton = GTK_WIDGET(gtk_builder_get_object(builder, "mainMenuBtn"));
    self->resetButton = GTK_WIDGET(gtk_builder_get_object(builder, "restartBtn"));
    self->rollbackButton = GTK_WIDGET(gtk_builder_get_object(builder, "rollbackBtn"));
    self->boardAnchorPoint = GTK_CONTAINER(gtk_builder_get_object(builder, "boardAnchorPoint"));
    self->scoreAnchorPoint = GTK_CONTAINER(gtk_builder_get_object(builder, "scoreAnchorPoint"));

}
void private_configureCallbacks(MatchView* self)
{
    g_signal_connect(self->mainMenuButton, "clicked", G_CALLBACK(private_mainMenu), self->controllerAPI);
    g_signal_connect(self->resetButton, "clicked", G_CALLBACK(private_restartGame), self->controllerAPI);
    g_signal_connect(self->rollbackButton, "clicked", G_CALLBACK(private_rollbackJump), self->controllerAPI);
}

MatchView *MatchView_new(IGameController* controllerAPI, MatchViewModel viewModel)
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
    gtk_widget_destroy(self->window);
    BoardView_destroy(self->boardView);
    ScoreView_destroy(self->scoreView);
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
