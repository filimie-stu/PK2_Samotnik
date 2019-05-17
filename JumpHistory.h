#pragma once
#include "JumpInfo.h"
#include "Observable.h"
typedef struct jump_history JumpHistory;

JumpHistory* JumpHistory_new(Observable* boardObservable);
void JumpHistory_destroy(JumpHistory* self);
JumpInfo JumpHistory_extract(JumpHistory* self);
void JumpHistory_addRecord(JumpHistory* self, JumpInfo jumpData);