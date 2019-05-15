#pragma once
#include "Vector2D.h"
typedef struct board_view BoardView;
typedef struct view_click_data 
{
    BoardView* view;
    Vector2D coords;
} ViewClickData;

ViewClickData* ViewClickData_new(BoardView* view, Vector2D coords);
void ViewClickData_destroy(ViewClickData* self);