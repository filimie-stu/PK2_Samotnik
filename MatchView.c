#include "MatchView.h"
#include "GameOverView.h"
#include "BoardView.h"
#include "ScoreView.h"
#include "Board.h"
#include "Observer.h"
#include "SyncScoreArgs.h"
#include <stdlib.h>
#include <string.h>

void GameController_mainMenu(GameController *self);
void GameController_restartGame(GameController *self);
void GameController_clickBoard(GameController *self, Vector2D coords);

typedef struct match_view
{
    GtkWidget *window;
    
    GtkWidget *mainMenuButton;
    GtkWidget *resetButton;

    BoardView* boardView;
    ScoreView* scoreView;
    GameOverView *gameOverDialog;
    Observer* boardObserver;
    GameController *controllerAPI;
    Observer *scoreObserver;

} MatchView;

static void private_mainMenu(GtkButton *button, gpointer data);
static void private_restartGame(GtkButton *button, gpointer data);

void private_recieveSignal(void *vSelf, const char *signalID, void *signalArgs)
{
    if (strncmp(signalID, "sync_score", strlen(signalID)) == 0)
    {
        MatchView *self = (MatchView *)vSelf;
        SyncScoreArgs args = *(SyncScoreArgs*)signalArgs;
        ScoreView_syncScore(self->scoreView, args);
    }
    else if (strncmp(signalID, "dead_end", strlen(signalID)) == 0)
    {
        MatchView *self = (MatchView *)vSelf;
        GameOverViewModel viewModel = { "You lose!", "There are no more possible moves" };
        self->gameOverDialog = GameOverView_new(self->controllerAPI, GTK_WINDOW(self->window), viewModel);
        GameOverView_display(self->gameOverDialog);
    }
    else if (strncmp(signalID, "goal_achieved", strlen(signalID)) == 0)
    {
        MatchView *self = (MatchView *)vSelf;
        GameOverViewModel viewModel = { "You win!", "There's only one token left, congratulations!" };
        self->gameOverDialog = GameOverView_new(self->controllerAPI, GTK_WINDOW(self->window), viewModel);
        GameOverView_display(self->gameOverDialog);
    }
}

MatchView *MatchView_new(GameController *controllerAPI, Board *board, Score* score)
{
    MatchView *created = (MatchView *)malloc(sizeof(MatchView));
    created->controllerAPI = controllerAPI;
    created->boardObserver = Observer_new(created, private_recieveSignal, board->observable);
    created->scoreObserver = Observer_new(created, private_recieveSignal, score->observable);
    GtkBuilder *builder = gtk_builder_new();

    GError *error = NULL;
    if (gtk_builder_add_from_file(builder, "view/in_game_view.glade", &error) == 0)
    {
        g_printerr("Error during loading file: %s\n", error->message);
        g_clear_error(&error);
    }

    created->window = GTK_WIDGET(gtk_builder_get_object(builder, "inGameWindow"));
    created->mainMenuButton = GTK_WIDGET(gtk_builder_get_object(builder, "mainMenuBtn"));

    g_signal_connect(created->mainMenuButton, "clicked", G_CALLBACK(private_mainMenu), controllerAPI);

    created->resetButton = GTK_WIDGET(gtk_builder_get_object(builder, "restartBtn"));
    g_signal_connect(created->resetButton, "clicked", G_CALLBACK(private_restartGame), controllerAPI);

    GtkContainer *boardAnchorPoint = GTK_CONTAINER(gtk_builder_get_object(builder, "boardAnchorPoint"));
    created->boardView = BoardView_new(controllerAPI, board, boardAnchorPoint);

    GtkContainer *scoreAnchorPoint = GTK_CONTAINER(gtk_builder_get_object(builder, "scoreAnchorPoint"));
    created->scoreView = ScoreView_new(controllerAPI, score, scoreAnchorPoint);


    return created;
}
void MatchView_destroy(MatchView *self)
{
    gtk_widget_destroy(self->window);
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
    GameController_mainMenu((GameController *)data);
}
void private_restartGame(GtkButton *button, gpointer data)
{
    GameController_restartGame((GameController *)data);
}