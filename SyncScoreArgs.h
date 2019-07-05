#pragma once


/**
 * \brief Struktura grupująca dane przesyłane z modelu wyniku do komplementarnego widoku.
 * 
 * Każda zmiana modelu jest równoznaczna z powiadomieniem widoku o konieczności aktualizacji. Ta struktura
 * grupuje dane, które zostaną w tym celu przekazane widokowi.
*/
typedef struct sync_score_args 
{
    int takedowns;              //!< liczba zbitych pionków.
    int tokensLeft;             //!< liczba pionków pozostałych do zbicia.
} SyncScoreArgs;