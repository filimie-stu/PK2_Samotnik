#include "MatchView.h"
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
    GtkWidget *boardGrid;
    GtkWidget *mainMenuButton;
    GtkWidget *resetButton;
    GtkWidget *tokensLeftCounter;
    GtkWidget *takedownsCounter;

    GameController *controllerAPI;
    Observer *boardObserver;
    Observer *scoreObserver;

} MatchView;

static void private_mainMenu(GtkButton *button, gpointer data);
static void private_restartGame(GtkButton *button, gpointer data);
static void private_loadModel(MatchView *self, GtkContainer *anchorPoint, Board *model);

void private_attachNewGridField(MatchView *self, int x, int y, const gchar *label)
{
    GtkWidget *button = gtk_button_new_with_label(label);
    gtk_grid_attach(GTK_GRID(self->boardGrid), button, x, y, 1, 1);
}

void private_boardClicked(GtkButton *button, gpointer data)
{
    ClickEvent *event = (ClickEvent *)data;
    GameController_clickBoard((GameController *)(event->args), event->coords);
}
void private_configureBoardClickCallback(MatchView *self, int row, int column)
{
    GtkWidget *clickedField = gtk_grid_get_child_at(GTK_GRID(self->boardGrid), column, row);

    Vector2D coords = {row, column};
    ClickEvent *clickEvent = ClickEvent_new(self->controllerAPI, coords);

    g_signal_connect_data(
        clickedField,
        "clicked",
        G_CALLBACK(private_boardClicked),
        clickEvent,
        (GClosureNotify)ClickEvent_destroy,
        0);
}

void private_loadModel(MatchView *self, GtkContainer *anchorPoint, Board *model)
{
    self->boardGrid = gtk_grid_new();
    gtk_container_add(anchorPoint, self->boardGrid);

    for (int i = 0; i < model->dimensions.y; i++)
    {
        for (int j = 0; j < model->dimensions.x; j++)
        {
            if (FieldType_toChar(model->fields[i][j].contents) != 'o' &&
                FieldType_toChar(model->fields[i][j].contents) != '_')
                continue;

            gchar label[2] = {FieldType_toChar(model->fields[i][j].contents), '\0'};
            private_attachNewGridField(self, j, i, label);
            private_configureBoardClickCallback(self, i, j);
        }
    }
}


void private_updateAt(MatchView* self, Vector2D at, const char* newLabel)
{
    GtkWidget* cell = gtk_grid_get_child_at(GTK_GRID(self->boardGrid), at.y, at.x);
    if (cell)
        gtk_button_set_label(GTK_BUTTON(cell), newLabel);
}

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

        private_updateAt(self, args.previouslyActive, "o");
        private_updateAt(self, args.activated, "O");
    }
    else if (strncmp(signalID, "jump", strlen(signalID)) == 0)
    {
        MatchView *self = (MatchView *)vSelf;
        JumpArgs args = *(JumpArgs*)signalArgs;
        private_updateAt(self, args.from, "_");
        private_updateAt(self, args.through, "_");
        private_updateAt(self, args.to, "o");
    }
    else if (strncmp(signalID, "sync_score", strlen(signalID)) == 0)
    {
        MatchView *self = (MatchView *)vSelf;
        SyncScoreArgs args = *(SyncScoreArgs*)signalArgs;
        private_syncScore(self, args);
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
    if (gtk_builder_add_from_file(builder, "view/match_view.xml", &error) == 0)
    {
        g_printerr("Error during loading file: %s\n", error->message);
        g_clear_error(&error);
    }

    created->window = GTK_WIDGET(gtk_builder_get_object(builder, "matchWindow"));
    created->takedownsCounter = GTK_WIDGET(gtk_builder_get_object(builder, "takedownsCounter"));
    created->tokensLeftCounter = GTK_WIDGET(gtk_builder_get_object(builder, "tokensLeftCounter"));
    created->mainMenuButton = GTK_WIDGET(gtk_builder_get_object(builder, "mainMenuButton"));
    g_signal_connect(created->mainMenuButton, "clicked", G_CALLBACK(private_mainMenu), controllerAPI);

    created->resetButton = GTK_WIDGET(gtk_builder_get_object(builder, "resetButton"));
    g_signal_connect(created->resetButton, "clicked", G_CALLBACK(private_restartGame), controllerAPI);

    GtkContainer *boardAnchorPoint = GTK_CONTAINER(gtk_builder_get_object(builder, "boardAnchorPoint"));
    private_loadModel(created, boardAnchorPoint, board);
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