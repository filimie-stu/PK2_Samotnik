#pragma once
#include "Vector2D.h"

typedef struct click_event
{
    void* args;
    Vector2D coords;

} ClickEvent;

ClickEvent* ClickEvent_new(void* args, Vector2D coords);
void ClickEvent_destroy(ClickEvent* self);