#include "Common.h"
#include <stdio.h>

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
        printf("%c: Unrecognized field type character. Defaulting to empty field.\n", c);
        return EMPTY;
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
            return ' ';
        case FORBIDDEN:
            return '#';
        default:
            printf("%d: Error: Unrecognized field type. Failed to convert to char.\n", (int)ft);
            return '?';
    }
}
const char* FieldType_toString(FieldType ft)
{
    switch(ft)
    {
        case REGULAR_TOKEN:
            return "o";
        case ACTIVE_TOKEN:
            return "O";
        case EMPTY:
            return " ";
        case FORBIDDEN:
            return "#";
        default:
            printf("%d: Unrecognized field type. Failed to convert to string.\n", (int)ft);
            return "?";
    }
}