#include "ScoreView.h"
#include "SyncScoreArgs.h"
#include <stdlib.h>
#include "Observer.h"
typedef struct score_view 
{
    GameController* controllerAPI;
    GtkContainer* parent;
    GtkWidget* scoreFrame;
    GtkWidget* takedownsCounter;
    GtkWidget* tokensLeftCounter;
    Observer* scoreObserver;

} ScoreView;
static void private_recieveSignal(void *vSelf, const char *signalID, void *signalArgs);
static void private_syncScore(ScoreView* self, SyncScoreArgs args);

void private_recieveSignal(void *vSelf, const char *signalID, void *signalArgs)
{
    if (strncmp(signalID, "sync_score", strlen(signalID)) == 0)
    {
        ScoreView *self = (ScoreView *)vSelf;
        SyncScoreArgs args = *(SyncScoreArgs*)signalArgs;
        private_syncScore(self, args);
    }
}

void private_syncScore(ScoreView* self, SyncScoreArgs args)
{
    char takedownsStr[3] = { '\0','\0','\0' };
    sprintf_s(takedownsStr, sizeof(takedownsStr), "%d", args.takedowns);
    gtk_label_set_label(GTK_LABEL(self->takedownsCounter), takedownsStr);
    
    char leftStr[3] = { '\0','\0','\0' };
    sprintf_s(leftStr, sizeof(leftStr), "%d", args.tokensLeft);
    gtk_label_set_label(GTK_LABEL(self->tokensLeftCounter), leftStr);
}

void private_loadModel(ScoreView* self, ScoreViewModel score)
{
    GtkBuilder *builder = gtk_builder_new_from_file("view/score_partial_view.xml");

    self->scoreFrame = GTK_WIDGET(gtk_builder_get_object(builder, "scoreFrame"));
    gtk_container_add(GTK_CONTAINER(self->parent), self->scoreFrame);
    gtk_container_child_set(GTK_CONTAINER(self->parent), self->scoreFrame, "fill", 1, "expand", 1, NULL);
    
    self->takedownsCounter = GTK_WIDGET(gtk_builder_get_object(builder, "takedownsCounter"));
    self->tokensLeftCounter = GTK_WIDGET(gtk_builder_get_object(builder, "tokensLeftCounter"));
    
    SyncScoreArgs initialScore = { score.points, score.goal - score.points };
    private_syncScore(self, initialScore);
}

ScoreView* ScoreView_new(GameController* controllerAPI, ScoreViewModel score, GtkContainer* parent)
{
    ScoreView* created = (ScoreView*)malloc(sizeof(ScoreView));

    created->parent = parent;
    created->controllerAPI = controllerAPI;
    created->scoreObserver = Observer_new(created, private_recieveSignal, score.scoreObservable);
    private_loadModel(created, score);
    return created;
}

void ScoreView_destroy(ScoreView* self)
{
    free(self);
}
void ScoreView_display(ScoreView* self)
{
    if (!self->scoreFrame)
        return;
    
    gtk_widget_show_all(self->scoreFrame);
}
void ScoreView_hide(ScoreView* self)
{
    if (!self->scoreFrame)
        return;
    
    gtk_widget_hide(self->scoreFrame);
}