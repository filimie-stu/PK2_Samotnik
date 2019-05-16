#include "Field.h"
#include <stdio.h>

Field Field_create(int x, int y, FieldType contents)
{
    Field created;
    created.coords.x = x;
    created.coords.y = y;
    created.contents = contents;

    return created;
}
