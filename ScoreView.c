#include "ScoreView.h"
#include "SyncScoreArgs.h"
#include <stdlib.h>
#include "Observer.h"

/**
 * \brief Składowa część głównego widoku rozgrywki - subwidok wyniku.
*/
typedef struct score_view 
{
    Observer* scoreObserver;            //!< obserwator modelu.
    IGameController* controllerAPI;     //!< publiczny interfejs kontrolera gry

    GtkContainer* parent;               //!< wskaźnik na obiekt biblioteki GTK+ reprezentujący kontener zawierający ten subwidok. 
    GtkWidget* scoreFrame;              //!< wskaźnik na obiekt biblioteki GTK+ reprezentujący ramkę z wynikiem. 
    GtkWidget* takedownsCounter;        //!< wskaźnik na obiekt biblioteki GTK+ reprezentujący liczbę zbitych pionków. 
    GtkWidget* tokensLeftCounter;       //!< wskaźnik na obiekt biblioteki GTK+ reprezentujący liczbę pionków pozostałych do zbicia. 
} ScoreView;


static void private_recieveSignal(void *vSelf, const char *signalID, void *signalArgs);
static void private_syncScore(ScoreView* self, SyncScoreArgs args);
static void private_setInitialValues(ScoreView* self, ScoreViewModel viewModel);
static void private_loadMembersFromXML(ScoreView* self);


void ScoreView_destroy(ScoreView* self)
{
    Observer_destroy(self->scoreObserver);
    free(self);
}
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
    sprintf(takedownsStr,"%d", args.takedowns);
    gtk_label_set_label(GTK_LABEL(self->takedownsCounter), takedownsStr);
    
    char leftStr[3] = { '\0','\0','\0' };
    sprintf(leftStr, "%d", args.tokensLeft);
    gtk_label_set_label(GTK_LABEL(self->tokensLeftCounter), leftStr);
}

void private_setInitialValues(ScoreView* self, ScoreViewModel viewModel)
{
    SyncScoreArgs initialScore = { viewModel.points, viewModel.goal - viewModel.points };
    private_syncScore(self, initialScore);
}
void private_loadMembersFromXML(ScoreView* self)
{
    GtkBuilder *builder = gtk_builder_new_from_file("view/score_partial_view.xml");
    
    self->scoreFrame = GTK_WIDGET(gtk_builder_get_object(builder, "scoreFrame"));
    gtk_container_add(GTK_CONTAINER(self->parent), self->scoreFrame);
    gtk_container_child_set(GTK_CONTAINER(self->parent), self->scoreFrame, "fill", 1, "expand", 1, NULL);
    
    self->takedownsCounter = GTK_WIDGET(gtk_builder_get_object(builder, "takedownsCounter"));
    self->tokensLeftCounter = GTK_WIDGET(gtk_builder_get_object(builder, "tokensLeftCounter"));
}

ScoreView* ScoreView_new(IGameController* controllerAPI, ScoreViewModel score, GtkContainer* parent)
{
    ScoreView* created = (ScoreView*)malloc(sizeof(ScoreView));

    created->parent = parent;
    created->controllerAPI = controllerAPI;
    created->scoreObserver = Observer_new(created, private_recieveSignal, score.scoreObservable);
    
    private_loadMembersFromXML(created);
    private_setInitialValues(created, score);

    return created;
}

