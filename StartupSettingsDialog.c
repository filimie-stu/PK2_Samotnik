#include "StartupSettingsDialog.h"
#include <stdlib.h>

typedef struct startup_settings_dialog
{
    GtkWidget* dialog;
    GtkWidget* filePickerBtn;
    GtkWidget* handicapSpinnerBtn;

} StartupSettingsDialog;


StartupSettingsDialog *StartupSettingsDialog_newFromFile(const char* relativeFilename, GtkWindow* parentWindow)
{
    StartupSettingsDialog* created = (StartupSettingsDialog*)malloc(sizeof(StartupSettingsDialog));
    GtkBuilder* builder = gtk_builder_new_from_file(relativeFilename);

    created->dialog = GTK_WIDGET(gtk_builder_get_object(builder,"startupSettingsDialog"));
    created->filePickerBtn = GTK_WIDGET(gtk_builder_get_object(builder,"boardChooserBtn"));
    created->handicapSpinnerBtn = GTK_WIDGET(gtk_builder_get_object(builder,"handicapBtn"));

    gtk_window_set_transient_for(GTK_WINDOW(created->dialog), parentWindow);
    gtk_spin_button_set_range(GTK_SPIN_BUTTON(created->handicapSpinnerBtn), 0, 5);
    gtk_spin_button_set_increments(GTK_SPIN_BUTTON(created->handicapSpinnerBtn), 1, 1);
    return created;
}
void StartupSettingsDialog_destroy(StartupSettingsDialog* self)
{
    free(self);
}

GtkWidget* StartupSettingsDialog_getDialog(StartupSettingsDialog* self)
{
    return self->dialog;
}

int StartupSettingsDialog_getHandicap(StartupSettingsDialog* self)
{
    return gtk_spin_button_get_value(GTK_SPIN_BUTTON(self->handicapSpinnerBtn));
}

const char* StartupSettingsDialog_getFilename(StartupSettingsDialog* self)
{
    return gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(self->filePickerBtn));
}