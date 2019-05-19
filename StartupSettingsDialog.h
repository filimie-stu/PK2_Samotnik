#pragma once
#include <gtk/gtk.h>


typedef struct startup_settings_dialog StartupSettingsDialog;

StartupSettingsDialog* StartupSettingsDialog_newFromFile(const char* relativeFilename, GtkWindow* parentWindow);
void StartupSettingsDialog_destroy(StartupSettingsDialog* self);
GtkWidget* StartupSettingsDialog_getDialog(StartupSettingsDialog* self);
int StartupSettingsDialog_getHandicap(StartupSettingsDialog* self);
const char* StartupSettingsDialog_getFilename(StartupSettingsDialog* self);