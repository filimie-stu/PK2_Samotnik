#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>


typedef struct 
{
    GtkStack* stack;
    GtkWidget* newPage;

} StackPageChange;
void exitProgram(GtkButton* btn, gpointer data)
{
    gtk_main_quit();
}

void beginMatch(GtkButton* btn, gpointer data)
{
    GtkWidget* mainMenuWindow = GTK_WIDGET(data);
    gtk_widget_hide(mainMenuWindow);


    GtkBuilder* builder = gtk_builder_new();
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "view/match_view.xml", &error) == 0)
    {
        g_printerr("Error during loading file: %s\n", error->message);
        g_clear_error(&error);
    }
    GtkWidget* window = GTK_WIDGET(gtk_builder_get_object(builder, "matchWindow"));
    gtk_widget_show_all(window);

}

int main(int argc, char* argv[])
{
    gtk_init(&argc, &argv);

    GtkBuilder* builder = gtk_builder_new();
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "view/main_menu_view.xml", &error) == 0)
    {
        g_printerr("Error during loading file: %s\n", error->message);
        g_clear_error(&error);
    }
    GtkWidget* window = GTK_WIDGET(gtk_builder_get_object(builder, "mainMenuWindow"));
    GtkWidget* newGameButton = GTK_WIDGET(gtk_builder_get_object(builder, "newGameButton"));
    GtkWidget* exitButton = GTK_WIDGET(gtk_builder_get_object(builder, "exitButton"));
    g_signal_connect(exitButton, "clicked", G_CALLBACK(exitProgram), NULL);

    g_signal_connect(newGameButton, "clicked", G_CALLBACK(beginMatch), window);



    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}