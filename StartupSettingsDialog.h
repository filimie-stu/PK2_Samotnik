#pragma once
#include <gtk/gtk.h>


typedef struct startup_settings_dialog StartupSettingsDialog;

/** \memberof startup_settings_dialog */
StartupSettingsDialog* StartupSettingsDialog_newFromFile(const char* relativeFilename, GtkWindow* parentWindow);

/** \memberof startup_settings_dialog */
void StartupSettingsDialog_destroy(StartupSettingsDialog* self);

/** \memberof startup_settings_dialog */
GtkWidget* StartupSettingsDialog_getDialog(StartupSettingsDialog* self);

/** \memberof startup_settings_dialog */
int StartupSettingsDialog_getHandicap(StartupSettingsDialog* self);

/** \memberof startup_settings_dialog */
const char* StartupSettingsDialog_getFilename(StartupSettingsDialog* self);