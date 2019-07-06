#include "IView.h"
#include "NewGameArgs.h"
#include "MainMenuViewModel.h"
#include "MainMenuView.h"
#include "HowToPlayDialog.h"
#include "StartupSettingsDialog.h"
#include <stdlib.h>
#include <assert.h>

/**
 * \brief Widok głównego menu.
*/
typedef struct main_menu_view
{
    IView *iView;                               //!< implementacja interfejsu IView .
    GtkWidget *window;                          //!< wskaźnik na obiekt biblioteki GTK+ reprezentujący główne okno programu.
    GtkWidget *continueBtn;                     //!< wskaźnik na obiekt biblioteki GTK+ reprezentujący przycisk kontynuacji gry. 
    int displayContinueBtn;                     //!< flaga informująca o tym, czy przycisk kontynuacji powinien zostać wyświetlony
    GtkWidget *newGameBtn;                      //!< wskaźnik na obiekt biblioteki GTK+ reprezentujący przycisk rozpoczęcia nowej gry. 
    GtkWidget *howToPlayBtn;                    //!< wskaźnik na obiekt biblioteki GTK+ reprezentujący przycisk wyświetlający instrukcję. 
    GtkWidget *exitBtn;                         //!< wskaźnik na obiekt biblioteki GTK+ reprezentujący przycisk zakończenia pracy programu. 
    StartupSettingsDialog *startupSettings;     //!< ustawienia wykorzystane ostatnim grazem do inicjalizacji sesji gry.
    HowToPlayDialog *howToPlayDialog;           //!< wskaźnik na okno dialogowe zawierające instrukcję do gry.
    IGameController *controllerAPI;             //!< publiczny interfejs kontrolera gry.
} MainMenuView;

static void private_exitProgram(IGameController *controller);
static void private_continueMatch(IGameController *controller);
static void private_newGame(MainMenuView *self, GtkButton *button);
static void private_howToPlay(MainMenuView *self);
static void private_loadMembersFromXML(MainMenuView *self, const char *relativeFilename);
static void private_configureCallbacks(MainMenuView *self);
static void private_loadCssFromFile(const char *relativeFilename);

static void private_wrapper_destroy(void *vSelf);
static void private_wrapper_display(void *vSelf);
static void private_wrapper_hide(void *vSelf);

MainMenuView *MainMenuView_new(IGameController *controllerAPI, MainMenuViewModel viewModel)
{
    MainMenuView *created = (MainMenuView *)malloc(sizeof(MainMenuView));
    created->iView = IView_new(
        created,
        private_wrapper_destroy,
        private_wrapper_display,
        private_wrapper_hide);
    created->controllerAPI = controllerAPI;

    private_loadMembersFromXML(created, "view/main_menu_view.glade");
    created->howToPlayDialog = HowToPlayDialog_newFromFile("view/how_to_play_view.glade", GTK_WINDOW(created->window));
    created->startupSettings = StartupSettingsDialog_newFromFile("view/startup_settings_view.glade", GTK_WINDOW(created->window));
    created->displayContinueBtn = viewModel.displayContinueButton;
    private_configureCallbacks(created);
    private_loadCssFromFile("css/theme.css");

    return created;
}
IView* MainMenuView_asIView(MainMenuView* self)
{
    return self->iView;
}
void MainMenuView_destroy(MainMenuView *self)
{
    gtk_widget_destroy(self->window);
    StartupSettingsDialog_destroy(self->startupSettings);
    HowToPlayDialog_destroy(self->howToPlayDialog);
    free(self);
}
void MainMenuView_display(MainMenuView *self)
{
    gtk_widget_show_all(self->window);
    if (!self->displayContinueBtn)
    {
        gtk_widget_hide(self->continueBtn);
    }
}
void MainMenuView_hide(MainMenuView *self)
{
    gtk_widget_hide(self->window);
}

void private_wrapper_destroy(void *vSelf)
{
    MainMenuView_destroy((MainMenuView *)vSelf);
}
void private_wrapper_display(void *vSelf)
{
    MainMenuView_display((MainMenuView *)vSelf);
}
void private_wrapper_hide(void *vSelf)
{
    MainMenuView_hide((MainMenuView *)vSelf);
}
void private_loadMembersFromXML(MainMenuView *self, const char *relativeFilename)
{
    GtkBuilder *builder = gtk_builder_new_from_file(relativeFilename);

    self->window = GTK_WIDGET(gtk_builder_get_object(builder, "mainMenuWindow"));
    self->exitBtn = GTK_WIDGET(gtk_builder_get_object(builder, "exitBtn"));
    self->newGameBtn = GTK_WIDGET(gtk_builder_get_object(builder, "newGameBtn"));
    self->howToPlayBtn = GTK_WIDGET(gtk_builder_get_object(builder, "howToPlayBtn"));
    self->continueBtn = GTK_WIDGET(gtk_builder_get_object(builder, "continueBtn"));
    g_object_unref(builder);
        
}

void private_configureCallbacks(MainMenuView *self)
{
    g_signal_connect_swapped(self->exitBtn, "clicked", G_CALLBACK(private_exitProgram), self->controllerAPI);
    g_signal_connect_swapped(self->continueBtn, "clicked", G_CALLBACK(private_continueMatch), self->controllerAPI);
    g_signal_connect_swapped(self->howToPlayBtn, "clicked", G_CALLBACK(private_howToPlay), self);
    g_signal_connect_swapped(self->newGameBtn, "clicked", G_CALLBACK(private_newGame), self);
    g_signal_connect(self->window, "delete-event", gtk_main_quit, NULL);
}

void private_loadCssFromFile(const char *relativeFilename)
{
    GFile *stylesheet = g_file_new_for_path(relativeFilename);
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_file(cssProvider, stylesheet, NULL);
    gtk_style_context_add_provider_for_screen(
        gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(cssProvider),
        GTK_STYLE_PROVIDER_PRIORITY_USER);
}
void private_howToPlay(MainMenuView *self)
{
    gtk_dialog_run(GTK_DIALOG(HowToPlayDialog_getDialog(self->howToPlayDialog)));
    gtk_widget_hide(HowToPlayDialog_getDialog(self->howToPlayDialog));
}
void private_newGame(MainMenuView *self, GtkButton *button)
{
    gint dialogResponse = gtk_dialog_run(GTK_DIALOG(StartupSettingsDialog_getDialog(self->startupSettings)));

    if (dialogResponse == GTK_RESPONSE_ACCEPT)
    {
        NewGameArgs args = {
            StartupSettingsDialog_getFilename(self->startupSettings),
            StartupSettingsDialog_getHandicap(self->startupSettings)};
        IGameController_beginMatch(self->controllerAPI, args);
    }
    else if (dialogResponse == GTK_RESPONSE_CANCEL || dialogResponse == GTK_RESPONSE_DELETE_EVENT)
    {
        gtk_widget_hide(StartupSettingsDialog_getDialog(self->startupSettings));
    }
    else
    {
        assert(0 && "We should've gotten here!\n");
    }
}
void private_continueMatch(IGameController *controller)
{
    IGameController_continueMatch(controller);
}
void private_exitProgram(IGameController *controller)
{
    IGameController_prepareForExit(controller);
    gtk_main_quit();
}
