#pragma once
#include "Vector2D.h"
#include "Common.h"


/**
 * \brief Struktura przechowująca współrzędne i typ pola.
*/
typedef struct field
{
    Vector2D coords;        //!< wspołrzędne pola
    FieldType contents;     //!< typ (zawartość) pola
} Field;

/**
 * \brief Tworzy nowy obiekt typu Field.
*/
Field Field_create(int x, int y, FieldType contents);

