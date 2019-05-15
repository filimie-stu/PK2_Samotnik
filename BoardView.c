#include "ActivateTokenArgs.h"
#include "ViewClickData.h"
#include "BoardView.h"
#include "JumpArgs.h"
#include "Vector2D.h"
#include "ClickEvent.h"
#include <stdlib.h>
#include <gtk/gtk.h>

typedef struct board_view
{
    Observer *observer;

    GameController *controllerAPI;
    GtkContainer *parent;
    GtkWidget *boardGrid;

    Vector2D activeField;
    Vector2D jumpSpots[4];

} BoardView;

typedef enum view_field_type
{
    VIEW_TOKEN,
    VIEW_ACTIVE_TOKEN,
    VIEW_EMPTY,
    VIEW_JUMP_SPOT

} ViewFieldType;

static void private_updateAt(BoardView *self, Vector2D at, const char *newLabel);
static GtkWidget* private_fieldAt(BoardView* self, Vector2D at);
static ViewFieldType private_determineFieldType(BoardView *self, Vector2D fieldCoords);
static int private_isJumpSpot(BoardView* self, GtkWidget* field);
static void private_setActiveField(BoardView* self, Vector2D newField);
static void private_resetActiveField(BoardView* self);
static void private_setJumpSpots(BoardView* self, Vector2D newSpots[4]);
static void private_resetJumpSpots(BoardView* self);
static void private_jumpSpotsLightsOn(BoardView* self);
static void private_jumpSpotsLightsOff(BoardView* self);
static void private_loadModel(BoardView *self, Board *model);
static void private_attachNewGridField(BoardView *self, int x, int y, const gchar *label);
static void private_configureBoardClickCallback(BoardView *self, int row, int column);
static void private_boardClicked(GtkButton *button, gpointer data);
static void private_recieveSignal(void *vSelf, const char *signalID, void *signalArgs);

void GameController_clickBoard(GameController *self, Vector2D coords);
void GameController_jump(GameController* self, Vector2D from, Vector2D to);
void GameController_activate(GameController* self, Vector2D at);


BoardView *BoardView_new(GameController *controllerAPI, Board *board, GtkContainer *parent)
{
    BoardView *created = (BoardView *)malloc(sizeof(BoardView));
    created->controllerAPI = controllerAPI;
    created->parent = parent;
    created->observer = Observer_new(created, private_recieveSignal, board->observable);
    private_loadModel(created, board);

    return created;
}
void BoardView_destroy(BoardView *self)
{
    free(self);
}
void BoardView_display(BoardView *self)
{
    if (!self->boardGrid)
        return;

    gtk_widget_show_all(self->boardGrid);
}
void BoardView_hide(BoardView *self)
{
    if (!self->boardGrid)
        return;

    gtk_widget_hide(self->boardGrid);
}


GtkWidget* private_fieldAt(BoardView* self, Vector2D at)
{
    return gtk_grid_get_child_at(GTK_GRID(self->boardGrid), at.y,at.x);
}
void private_updateAt(BoardView *self, Vector2D at, const char *newLabel)
{
    GtkWidget *cell = gtk_grid_get_child_at(GTK_GRID(self->boardGrid), at.y, at.x);
    if (cell)
        gtk_button_set_label(GTK_BUTTON(cell), newLabel);
}
void private_setActiveField(BoardView* self, Vector2D newField)
{
    private_updateAt(self, self->activeField, "o");
    self->activeField = newField;
    private_updateAt(self, self->activeField, "O");
}
void private_resetActiveField(BoardView* self)
{
    self->activeField = Vector2D_create(-1,-1);
}
void private_setJumpSpots(BoardView* self, Vector2D newSpots[4])
{
    private_jumpSpotsLightsOff(self);
    for (int i = 0; i < 4; i++)
    {
        self->jumpSpots[i] = newSpots[i];
    }
    private_jumpSpotsLightsOn(self);
}
void private_resetJumpSpots(BoardView* self)
{
    private_jumpSpotsLightsOff(self);
    for (int i = 0; i < 4; i++)
        self->jumpSpots[i] = Vector2D_create(-1,-1);
}
void private_jumpSpotsLightsOff(BoardView* self)
{
    for (int i = 0; i < 4; i++)
    {
        GtkWidget* highlightedField = private_fieldAt(self, self->jumpSpots[i]); 
        if (highlightedField)
        {
            gtk_style_context_remove_class(
                gtk_widget_get_style_context(highlightedField), "jump-target");
        }
    }
}
void private_jumpSpotsLightsOn(BoardView* self)
{
    for (int i = 0; i < 4; i++)
    {
        GtkWidget* fieldToHighlight = private_fieldAt(self, self->jumpSpots[i]); 
        if (fieldToHighlight)

    gtk_style_context_add_class(
        gtk_widget_get_style_context(fieldToHighlight), "jump-target");
    }
    
}
ViewFieldType private_determineFieldType(BoardView *self, Vector2D fieldCoords)
{
    GtkWidget *field = private_fieldAt(self, fieldCoords);
    const gchar *label = gtk_button_get_label(GTK_BUTTON(field));

    if (private_isJumpSpot(self, field))
        return VIEW_JUMP_SPOT;
    else if (strncmp(label, "_", strlen(label))==0)
        return VIEW_EMPTY;
    else if (strncmp(label, "o", strlen(label))==0)
        return VIEW_TOKEN;
    else if (strncmp(label, "O", strlen(label))==0)
        return VIEW_ACTIVE_TOKEN;
    else
        assert(0 && "We should never get in here.\n");
}
int private_isJumpSpot(BoardView* self, GtkWidget* field)
{
    if (!private_fieldAt(self, self->activeField))
        return 0;

    for (int i = 0; i < 4; i++)
    {
        if (field == private_fieldAt(self, self->jumpSpots[i]))
            return 1;
    }   

    return 0;
}
void private_recieveSignal(void *vSelf, const char *signalID, void *signalArgs)
{
    if (strncmp(signalID, "activate", strlen(signalID)) == 0)
    {
        BoardView *self = (BoardView *)vSelf;
        ActivateTokenArgs *args = (ActivateTokenArgs *)signalArgs;
        private_setJumpSpots(self, args->jumpSpots);
        private_setActiveField(self, args->activeCoords);
    }
    else if (strncmp(signalID, "jump", strlen(signalID)) == 0)
    {
        BoardView *self = (BoardView *)vSelf;
        JumpInfo args = *(JumpInfo*)signalArgs;
        private_updateAt(self, args.from, "_");
        private_updateAt(self, args.through, "_");
        private_updateAt(self, args.to, "o");
        private_resetJumpSpots(self);
        private_resetActiveField(self);
    }
}
void private_boardClicked(GtkButton *button, gpointer data)
{
    ViewClickData *clickData = (ViewClickData *)data;
    BoardView* self = clickData->view;
    Vector2D clickCoords = clickData->coords;

    ViewFieldType type = private_determineFieldType(self, clickCoords);
    switch (type)
    {
    case VIEW_JUMP_SPOT:
        GameController_jump(self->controllerAPI, self->activeField, clickCoords);
        break;

    case VIEW_TOKEN:
        GameController_activate(self->controllerAPI, clickCoords);
        break;

    case VIEW_EMPTY:
    case VIEW_ACTIVE_TOKEN:
        break;

    default:
        assert(0 && "We should never get in here.\n");
        break;
    }
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
    ViewClickData *clickData = ViewClickData_new(self, coords);

    g_signal_connect_data(
        clickedField,
        "clicked",
        G_CALLBACK(private_boardClicked),
        clickData,
        (GClosureNotify)ViewClickData_destroy,
        0);
}