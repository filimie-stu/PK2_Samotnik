#include "BoardView.h"
#include "Vector2D.h"
#include "ClickEvent.h"
#include <stdlib.h>
#include <gtk/gtk.h>

typedef struct board_view 
{
    GameController* controllerAPI;
    GtkContainer* parent;
    GtkWidget* boardGrid;
} BoardView;

static void private_attachNewGridField(BoardView *self, int x, int y, const gchar *label);
static void private_boardClicked(GtkButton *button, gpointer data);
static void private_configureBoardClickCallback(BoardView *self, int row, int column);
static void private_loadModel(BoardView *self, Board *model);

void GameController_clickBoard(GameController* self, Vector2D coords);

BoardView* BoardView_new(GameController* controllerAPI, Board* board, GtkContainer* parent)
{
    BoardView* created = (BoardView*)malloc(sizeof(BoardView));
    created->controllerAPI = controllerAPI;
    created->parent = parent;

    private_loadModel(created, board);

    return created;
}
void BoardView_destroy(BoardView* self)
{
    free(self);
}
void BoardView_display(BoardView* self)
{
    if (!self->boardGrid)
        return;

    gtk_widget_show_all(self->boardGrid);
}
void BoardView_hide(BoardView* self)
{
    if (!self->boardGrid)
        return;

    gtk_widget_hide(self->boardGrid);
}
void BoardView_updateAt(BoardView* self, Vector2D at, const char* newLabel)
{
    GtkWidget* cell = gtk_grid_get_child_at(GTK_GRID(self->boardGrid), at.y, at.x);
    if (cell)
        gtk_button_set_label(GTK_BUTTON(cell), newLabel);
}


void private_boardClicked(GtkButton *button, gpointer data)
{
    ClickEvent *event = (ClickEvent *)data;
    GameController_clickBoard((GameController *)(event->args), event->coords);
}
void private_loadModel(BoardView *self, Board *model)
{
    self->boardGrid = gtk_grid_new();
    gtk_container_add(self->parent, self->boardGrid);

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
void private_attachNewGridField(BoardView *self, int x, int y, const gchar *label)
{
    GtkWidget *button = gtk_button_new_with_label(label);
    gtk_grid_attach(GTK_GRID(self->boardGrid), button, x, y, 1, 1);
}
void private_configureBoardClickCallback(BoardView *self, int row, int column)
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