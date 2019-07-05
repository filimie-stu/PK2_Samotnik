#include "JumpHistory.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#define MAX_RECORDS 200

/**
 * \brief Klasa odpowiedzialna za zarządzanie historią ruchów wykonanych w trakcie meczu.
 * 
 *  Zasadniczą część funkcjonalności tej klasy stanowi zarządzanie stosem obiektów typu JumpInfo - posiada więc ona metody:
 * "add" i "extract".
*/
typedef struct jump_history
{
    IJumpHistory *iJumpHistory;
    JumpInfo recordedJumps[MAX_RECORDS];
    int recordedCount;
} JumpHistory;

static void private_wrapper_destroy(void *vSelf);
static void private_wrapper_addRecord(void *vSelf, JumpInfo jumpData);
static JumpInfo private_wrapper_extract(void *vSelf);
static int private_wrapper_isEmpty(void *vSelf);

static int private_wrapper_isEmpty(void *vSelf)
{
    return JumpHistory_isEmpty((JumpHistory *)vSelf);
}
void private_wrapper_destroy(void *vSelf)
{
    //todo
}
void private_wrapper_addRecord(void *vSelf, JumpInfo jumpData)
{
    JumpHistory_addRecord((JumpHistory *)vSelf, jumpData);
}
JumpInfo private_wrapper_extract(void *vSelf)
{
    return JumpHistory_extract((JumpHistory *)vSelf);
}
int JumpHistory_isEmpty(JumpHistory *self)
{
    return self->recordedCount <= 0;
}
JumpHistory *JumpHistory_new()
{
    JumpHistory *created = (JumpHistory *)malloc(sizeof(JumpHistory));
    created->recordedCount = 0;
    created->iJumpHistory = IJumpHistory_new(
        created,
        private_wrapper_destroy,
        private_wrapper_addRecord,
        private_wrapper_isEmpty,
        private_wrapper_extract);
    return created;
}
void JumpHistory_destroy(JumpHistory *self)
{
    free(self);
}
IJumpHistory *JumpHistory_asIJumpHistory(JumpHistory *self)
{
    return self->iJumpHistory;
}
void JumpHistory_addRecord(JumpHistory *self, JumpInfo jump)
{
    assert(self->recordedCount < MAX_RECORDS);

    self->recordedJumps[self->recordedCount] = jump;
    self->recordedCount++;
}
JumpInfo JumpHistory_extract(JumpHistory *self)
{
    if (self->recordedCount <= 0)
        printf("Error: there are no recorded jumps to extract.\n");

    self->recordedCount--;
    return self->recordedJumps[self->recordedCount];
}