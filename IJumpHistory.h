#pragma once
#include "JumpInfo.h"

typedef struct i_jump_history IJumpHistory;


IJumpHistory *IJumpHistory_new(
    void *implementationObject,
    void (*destroyOverride)(void *implObject),
    void (*addRecordOverride)(void *implObject, JumpInfo jumpData),
    int (*isEmptyOverride)(void* implObject),
    JumpInfo (*extractOverride)(void *impleObject));
void IJumpHistory_destroy(IJumpHistory* self, int destroyDerivedTypes);
JumpInfo IJumpHistory_extract(IJumpHistory* self);
void IJumpHistory_addRecord(IJumpHistory* self, JumpInfo jumpData);
int IJumpHistory_isEmpty(IJumpHistory* self);