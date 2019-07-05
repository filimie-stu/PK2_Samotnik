#pragma once
#include "JumpInfo.h"
#include "Observable.h"
#include "IJumpHistory.h"
typedef struct jump_history JumpHistory;

/** \memberof jump_history */
JumpHistory* JumpHistory_new();

/** \memberof jump_history */
IJumpHistory* JumpHistory_asIJumpHistory(JumpHistory* self);

/** \memberof jump_history */
void JumpHistory_destroy(JumpHistory* self);

/**
 * \memberof jump_history
 * \brief Usuwa i zwraca ostatnio dodany na stos obiekt typu JumpInfo.
 * \return Ostatnio dodany na stos obiekt, teraz zeń usunięty.
*/
JumpInfo JumpHistory_extract(JumpHistory* self);

/**
 * \memberof jump_history
 * \brief Dodaje na szczyt stosu nowy obiekt z danymi dot. wykonanego skoku.
 * \param jumpData dane dot. wykonanego skoku.
*/
void JumpHistory_addRecord(JumpHistory* self, JumpInfo jumpData);

/** \memberof jump_history */
int JumpHistory_isEmpty(JumpHistory* self);