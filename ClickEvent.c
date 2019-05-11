#include "ClickEvent.h"
#include <stdlib.h>

ClickEvent* ClickEvent_new(void* args, Vector2D coords)
{
    ClickEvent* created = (ClickEvent*)malloc(sizeof(ClickEvent));
    created->args = args;
    created->coords = coords;
    return created;
}
void ClickEvent_destroy(ClickEvent* self)
{
    free(self);
}