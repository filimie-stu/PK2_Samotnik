#pragma once
#include "JumpInfo.h"

typedef struct i_jump_history IJumpHistory;


/** \memberof i_jump_history */
IJumpHistory *IJumpHistory_new(
    void *implementationObject,
    void (*destroyOverride)(void *implObject),
    void (*addRecordOverride)(void *implObject, JumpInfo jumpData),
    int (*isEmptyOverride)(void* implObject),
    JumpInfo (*extractOverride)(void *impleObject));
/** \memberof i_jump_history */
void IJumpHistory_destroy(IJumpHistory* self, int destroyDerivedTypes);
/** \memberof i_jump_history */
JumpInfo IJumpHistory_extract(IJumpHistory* self);
/** \memberof i_jump_history */
void IJumpHistory_addRecord(IJumpHistory* self, JumpInfo jumpData);
/** \memberof i_jump_history */
int IJumpHistory_isEmpty(IJumpHistory* self);