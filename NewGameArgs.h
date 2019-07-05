#pragma once

/**
 * \brief Struktura grupująca przekazywane kontrolerowi podstawowe ustawienia gry.
*/
typedef struct new_game_args
{
    const char *boardFilename;  //!< ścieżka do pliku, na podstawie którego utworzona zostanie plansza.
    int handicap;               //!< liczba pionków pomocniczych, których usunięcie z planszy nie będzie potrzebne do zwycięstwa.
} NewGameArgs;

