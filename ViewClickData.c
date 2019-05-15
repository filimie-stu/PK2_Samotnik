#include "ViewClickData.h"
#include <stdlib.h>

ViewClickData* ViewClickData_new(BoardView* view, Vector2D coords)
{
    ViewClickData* created = (ViewClickData*)malloc(sizeof(ViewClickData));
    created->view = view;
    created->coords = coords;
    return created;
}
void ViewClickData_destroy(ViewClickData* self)
{
    free(self);
}