#include "NewGameArgs.h"
#include "MainMenuView.h"
#include "HowToPlayView.h"
#include "StartupSettingsDialog.h"
#include <stdlib.h>
#include <assert.h>

void GameController_prepareForExit(GameController* self);
void GameController_mainMenu(GameController* self);
void GameController_beginMatch(GameController* self, NewGameArgs args);
void GameController_continueMatch(GameController* self);

typedef struct main_menu_view 
{
    GtkWidget* window;
    GtkWidget* continueBtn;
    GtkWidget* newGameBtn;
    GtkWidget* howToPlayBtn;
    GtkWidget* exitBtn;
    StartupSettingsDialog* startupSettings;
    GameController* controllerAPI;
    HowToPlayView* howToPlayView;

} MainMenuView;




static void private_beginMatch(GtkButton* button, gpointer data);
static void private_exitProgram(GtkButton* button, gpointer data);
static void private_continueMatch(GtkButton* button, gpointer data);
static void private_newGame(MainMenuView* self,GtkButton* button);

void private_newGame(MainMenuView* self, GtkButton* button)
{
    gint dialogResponse = gtk_dialog_run(GTK_DIALOG(StartupSettingsDialog_getDialog(self->startupSettings)));

    if (dialogResponse == GTK_RESPONSE_ACCEPT)
    {
        NewGameArgs args = { 
            StartupSettingsDialog_getFilename(self->startupSettings),
            StartupSettingsDialog_getHandicap(self->startupSettings)
             };
        GameController_beginMatch(self->controllerAPI, args); 
    }
    else if (dialogResponse == GTK_RESPONSE_CANCEL)
    {
        // just close the dialog
    }  
    else
    {
        assert(0 && "We should've gotten here!\n");
    }
}
void private_continueMatch(GtkButton* button, gpointer data)
{
    GameController_continueMatch((GameController*)data);
}

void private_displayHowToPlay(GtkButton* button, gpointer data)
{
    HowToPlayView_display((HowToPlayView*)data);
}
MainMenuView* MainMenuView_new(GameController* controllerAPI)
{
    MainMenuView* created = (MainMenuView*)malloc(sizeof(MainMenuView));
    created->startupSettings = StartupSettingsDialog_newFromFile("view/startup_settings_view.glade");
    created->controllerAPI = controllerAPI;

    GtkBuilder* builder = gtk_builder_new();
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "view/main_menu_view.glade", &error) == 0)
    {
        g_printerr("Error during loading file: %s\n", error->message);
        g_clear_error(&error);
    }
    created->window = GTK_WIDGET(gtk_builder_get_object(builder, "mainMenuWindow"));
    created->exitBtn = GTK_WIDGET(gtk_builder_get_object(builder, "exitBtn"));
    created->newGameBtn = GTK_WIDGET(gtk_builder_get_object(builder, "newGameBtn"));
    created->continueBtn = GTK_WIDGET(gtk_builder_get_object(builder, "continueBtn"));
    created->howToPlayBtn = GTK_WIDGET(gtk_builder_get_object(builder, "howToPlayBtn"));

    created->howToPlayView = HowToPlayView_new(controllerAPI, GTK_WINDOW(created->window));
    g_signal_connect(created->exitBtn, "clicked", G_CALLBACK(private_exitProgram), controllerAPI);
    // NewGameArgs* newGameArgs = NewGameArgs_new(
    //     controllerAPI,
    //     StartupSettingsDialog_getFilename(created->startupSettings),
    //     StartupSettingsDialog_getHandicap(created->startupSettings)
    // );
    g_signal_connect_swapped(created->newGameBtn, "clicked", G_CALLBACK(private_newGame), created);
    g_signal_connect(created->continueBtn, "clicked", G_CALLBACK(private_continueMatch), controllerAPI);
    g_signal_connect(created->howToPlayBtn, "clicked", G_CALLBACK(private_displayHowToPlay), created->howToPlayView);

    GFile* stylesheet = g_file_new_for_path("css/theme.css");
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_file(cssProvider, stylesheet, NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    return created;
}
void MainMenuView_destroy(MainMenuView* self)
{
    gtk_widget_destroy(self->window);
    free(self);
}
void MainMenuView_display(MainMenuView* self)
{
    gtk_widget_show_all(self->window);
}
void MainMenuView_displayContinueButton(MainMenuView* self)
{
    gtk_widget_show(self->continueBtn);
}
void MainMenuView_hideContinueButton(MainMenuView* self)
{
    gtk_widget_hide(self->continueBtn);
}
void MainMenuView_hide(MainMenuView* self)
{
    gtk_widget_hide(self->window);
}

void private_beginMatch(GtkButton* button, gpointer data)
{
    // GameController_beginMatch((GameController*)data);
}

void private_exitProgram(GtkButton* button, gpointer data)
{
    GameController_prepareForExit((GameController*)data);
    gtk_main_quit();
}
