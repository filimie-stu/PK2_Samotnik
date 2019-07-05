#pragma once
#include "IBoard.h"
#include "Vector2D.h"
#include "Field.h"
#include "Observable.h"
#include "JumpInfo.h"
#include "JumpHistory.h"

typedef enum
{
    NO_ACTION, JUMP, ACTIVATE_TOKEN
} ClickResult;


typedef struct board Board;

/** \memberof board */
Board* Board_newFromFile(const char* relativePath);
/** \memberof board */
void Board_destroy(Board* self);
/** \memberof board */
IBoard* Board_asIBoard(Board* self);
/** \memberof board */
Observable* Board_asObservable(Board* self);

/**
    \memberof board
    \brief Próba wykonania ruchu pionkiem.
    \param from współrzędne pionka oczekującego na przesunięcie.
    \param to współrzędne pustego pola, na którym za zostać ustawiony pionek.
    \param out_jumpData parametr wyjściowy - w przypadku udanego ruchu zawiera współrzędne startu i końca ruchu, a także zbitego pionka.
    \return 1 jeżeli operacja powiodła się, w przeciwnym wypadku - 0.
*/
int Board_tryJump(Board* self, Vector2D from, Vector2D to, JumpInfo* out_jumpData);

/*!
    \memberof board
    \brief Próba aktywacji pola.
    \param at współrzędne pola przeznaczonego do aktywacji.
    \return 1 jeżeli operacja powiodła się, w przeciwnym wypadku - 0.
*/
int Board_tryActivate(Board* self, Vector2D at);

/*!
    \memberof board
    \brief Cofnięcie ruchu.
    \param jumpData pełen zestaw informacji odnośnie cofanego ruchu: pole z którego wykonywano ruch, pole końcowe, położenie zbitego pionka. 
*/
void Board_rollbackJump(Board* self, JumpInfo jumpData);

/*! 
    \memberof board
    \brief Akcesor wymiarów planszy.
*/
Vector2D Board_getDimensions(Board* self);

/*!
    \memberof board
    \brief Akcesor typu pola o określonych współrzędnych.
*/
FieldType Board_getFieldAt(Board* self, Vector2D at);

/*! 
    \memberof board
    \brief Zliczenie pozostałych na planszy pionków.
*/
int Board_countTokens(Board* self);

/*!
    \memberof board
    \brief Sprawdzenie czy pozostały jakiekolwiek możliwe ruchy.
    \return 1 jeżeli nie ma już możliwych do wykonania ruchów, w przeciwnym wypadku - 0.
*/
int Board_isDeadEnd(Board* self);