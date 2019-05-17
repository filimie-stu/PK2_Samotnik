#include "JumpHistory.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#define MAX_RECORDS 30
typedef struct jump_history
{
    JumpInfo recordedJumps[MAX_RECORDS];
    int recordedCount;
} JumpHistory;

static void private_recieveSignal(void* vSelf, const char* signalID, void* signalArgs);

JumpHistory* JumpHistory_new(Observable* boardObservable)
{
    JumpHistory* created = (JumpHistory*)malloc(sizeof(JumpHistory));
    created->recordedCount = 0;
    return created;
}
void JumpHistory_destroy(JumpHistory* self)
{
    free(self);
}
void JumpHistory_addRecord(JumpHistory* self, JumpInfo jump)
{
    assert(self->recordedCount < MAX_RECORDS);

    self->recordedJumps[self->recordedCount] = jump;
    self->recordedCount++;
}
JumpInfo JumpHistory_extract(JumpHistory* self)
{
    if (self->recordedCount <= 0)
        printf("Error: there are no recorded jumps to extract.\n");

    self->recordedCount--;
    return self->recordedJumps[self->recordedCount];
}