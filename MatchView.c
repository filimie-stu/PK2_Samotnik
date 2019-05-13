#include "MatchView.h"
#include "GameOverView.h"
#include "BoardView.h"
#include "Board.h"
#include "ClickEvent.h"
#include "Observer.h"
#include "JumpArgs.h"
#include "ActivateArgs.h"
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
    GtkWidget *tokensLeftCounter;
    GtkWidget *takedownsCounter;
    GameOverView *gameOverDialog;
    BoardView* boardView;

    GameController *controllerAPI;
    Observer *boardObserver;
    Observer *scoreObserver;

} MatchView;

static void private_mainMenu(GtkButton *button, gpointer data);
static void private_restartGame(GtkButton *button, gpointer data);

void private_syncScore(MatchView* self, SyncScoreArgs args)
{
    char takedownsStr[3] = { '\0','\0','\0' };
    sprintf_s(takedownsStr, sizeof(takedownsStr), "%d", args.takedowns);
    gtk_label_set_label(GTK_LABEL(self->takedownsCounter), takedownsStr);
    
    char leftStr[3] = { '\0','\0','\0' };
    sprintf_s(leftStr, sizeof(leftStr), "%d", args.tokensLeft);
    gtk_label_set_label(GTK_LABEL(self->tokensLeftCounter), leftStr);
}

void private_recieveSignal(void *vSelf, const char *signalID, void *signalArgs)
{
    if (strncmp(signalID, "activate_token", strlen(signalID)) == 0)
    {
        MatchView *self = (MatchView *)vSelf;
        ActivateArgs args = *(ActivateArgs*)signalArgs;

        BoardView_updateAt(self->boardView, args.previouslyActive, "o");
        BoardView_updateAt(self->boardView, args.activated, "O");
    }
    else if (strncmp(signalID, "jump", strlen(signalID)) == 0)
    {
        MatchView *self = (MatchView *)vSelf;
        JumpArgs args = *(JumpArgs*)signalArgs;
        BoardView_updateAt(self->boardView, args.from, "_");
        BoardView_updateAt(self->boardView, args.through, "_");
        BoardView_updateAt(self->boardView, args.to, "o");
    }
    else if (strncmp(signalID, "sync_score", strlen(signalID)) == 0)
    {
        MatchView *self = (MatchView *)vSelf;
        SyncScoreArgs args = *(SyncScoreArgs*)signalArgs;
        private_syncScore(self, args);
    }
    // else if (strncmp(signalID, "dead_end", strlen(signalID)) == 0)
    // {
        // MatchView *self = (MatchView *)vSelf;
        // GameOverView_display(self->gameOverDialog);
    // }
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
    created->takedownsCounter = GTK_WIDGET(gtk_builder_get_object(builder, "takedownsCounter"));
    created->tokensLeftCounter = GTK_WIDGET(gtk_builder_get_object(builder, "tokensLeftCounter"));
    created->mainMenuButton = GTK_WIDGET(gtk_builder_get_object(builder, "mainMenuBtn"));
    created->gameOverDialog = GameOverView_new(controllerAPI, GTK_CONTAINER(created->window));
    g_signal_connect(created->mainMenuButton, "clicked", G_CALLBACK(private_mainMenu), controllerAPI);

    created->resetButton = GTK_WIDGET(gtk_builder_get_object(builder, "restartBtn"));
    g_signal_connect(created->resetButton, "clicked", G_CALLBACK(private_restartGame), controllerAPI);

    GtkContainer *boardAnchorPoint = GTK_CONTAINER(gtk_builder_get_object(builder, "boardAnchorPoint"));
    created->boardView = BoardView_new(controllerAPI, board, boardAnchorPoint);
    
    SyncScoreArgs initialScore = { score->takedowns, score->goal - score->takedowns };
    private_syncScore(created, initialScore);

    

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