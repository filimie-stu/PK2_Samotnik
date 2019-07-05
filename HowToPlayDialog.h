#pragma once
#include <gtk/gtk.h>

typedef struct game_controller GameController;
typedef struct how_to_play_view HowToPlayDialog;

/** \memberof how_to_play_view */
HowToPlayDialog* HowToPlayDialog_newFromFile(const char* relativeFilename, GtkWindow *parentWindow);

/** \memberof how_to_play_view */
void HowToPlayDialog_destroy(HowToPlayDialog* self);

/** \memberof how_to_play_view */
GtkWidget* HowToPlayDialog_getDialog(HowToPlayDialog* self);
