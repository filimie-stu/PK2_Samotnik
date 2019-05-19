#pragma once
#include <gtk/gtk.h>

typedef struct game_controller GameController;
typedef struct how_to_play_view HowToPlayDialog;

HowToPlayDialog* HowToPlayDialog_newFromFile(const char* relativeFilename, GtkWindow *parentWindow);
void HowToPlayDialog_destroy(HowToPlayDialog* self);
GtkWidget* HowToPlayDialog_getDialog(HowToPlayDialog* self);
