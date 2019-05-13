#pragma once
#include <gtk/gtk.h>

typedef struct game_controller GameController;
typedef struct how_to_play_view HowToPlayView;

HowToPlayView* HowToPlayView_new(GameController* controllerAPI, GtkWindow* parentWindow);
void HowToPlayView_destroy(HowToPlayView* self);
void HowToPlayView_display(HowToPlayView* self);
void HowToPlayView_hide(HowToPlayView* self);
