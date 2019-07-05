#pragma once
#include "Vector2D.h"

//! Struktura wysyłana z modelu do widoku, zawierająca współrzędne aktywowanego pola oraz pól, na które można z niego przesunąć pionek.
typedef struct activate_token_args
{
    Vector2D activeCoords; //!< współrzędne aktywowanego pola.
    Vector2D jumpSpots[4]; //!< tablica współrzędnych potencjalnych miejsc skoku. Jeżeli jest ich mniej niż 4 (a przeważnie tak jest), nadmiarowe miejsca wpisywany jest wektor współrzędnych [-1, -1]
} ActivateTokenArgs;