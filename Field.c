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
FieldType FieldType_fromChar(char c)
{
    switch (c)
    {
    case 'o':
        return REGULAR_TOKEN;
    case 'O':
        return ACTIVE_TOKEN;
    case '_':
        return EMPTY;
    case '#':
        return FORBIDDEN;
    default:
        printf("%c: ", c);
        perror("Unrecognized field type character.\n");
        return FORBIDDEN;
    }
}
char FieldType_toChar(FieldType ft)
{
    switch(ft)
    {
        case REGULAR_TOKEN:
            return 'o';
        case ACTIVE_TOKEN:
            return 'O';
        case EMPTY:
            return '_';
        case FORBIDDEN:
            return '#';
        default:
            perror("Unrecognized field type. Failed to convert to char.\n");
            return '?';
    }
}